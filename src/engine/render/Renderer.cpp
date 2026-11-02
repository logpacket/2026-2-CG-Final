#include "engine/render/Renderer.h"

#include "core/AssetPath.h"

#include <glad/glad.h>
#include <glm/exponential.hpp>
#include <glm/matrix.hpp>

namespace {

constexpr float kGamma = 2.2f;

// 화면에 보이는 색(sRGB)을 조명 계산에 쓰는 선형 값으로 바꾼다. basic.frag 마지막의 감마 보정과 짝이다.
// 모든 픽셀에서 같은 값이라 셰이더 대신 CPU 에서 한 번 바꾼다.
glm::vec3 srgbToLinear(const glm::vec3& color)
{
    return glm::pow(color, glm::vec3(kGamma));
}

}  // namespace

Renderer::Renderer() : shader_(assetPath("shaders/basic.vert"), assetPath("shaders/basic.frag")) {}

void Renderer::beginFrame(const Camera& camera, const Lighting& lighting, const glm::ivec2& framebufferSize,
                          const glm::vec3& clearColor) const
{
    glViewport(0, 0, framebufferSize.x, framebufferSize.y);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // ImGui 가 프레임마다 상태를 바꾸므로 앞의 상태를 가정하지 않고 매번 설정한다.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);

    const float aspect = framebufferSize.y > 0
                             ? static_cast<float>(framebufferSize.x) / static_cast<float>(framebufferSize.y)
                             : 1.0f;
    shader_.bind();
    shader_.setMat4("uViewProjection", camera.projection(aspect) * camera.view());
    shader_.setVec3("uCameraPositionWorld", camera.position);
    shader_.setVec3("uLightDirectionWorld", lighting.directional.direction);
    shader_.setVec3("uLightColor", srgbToLinear(lighting.directional.color) * lighting.directional.intensity);
    shader_.setVec3("uAmbientColor", srgbToLinear(lighting.ambientColor));
}

void Renderer::draw(const Mesh& mesh, const Material& material, const glm::mat4& model) const
{
    shader_.setMat4("uModel", model);
    // 크기가 축마다 달라도 노멀이 면에 수직으로 남도록 역행렬의 전치를 쓴다.
    shader_.setMat3("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
    shader_.setVec3("uBaseColor", srgbToLinear(material.baseColor));
    shader_.setFloat("uMetallic", material.metallic);
    shader_.setFloat("uRoughness", material.roughness);
    mesh.draw();
}
