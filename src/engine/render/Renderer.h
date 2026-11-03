// Blinn-Phong 셰이더 하나로 장면을 그린다. 프레임마다 beginFrame 을 한 번 부르고, 물체마다 draw 를 부른다.
#pragma once

#include "engine/gfx/Material.h"
#include "engine/gfx/Mesh.h"
#include "engine/gfx/Shader.h"
#include "engine/render/Light.h"
#include "engine/scene/Camera.h"

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// 확장: 그림자처럼 패스가 늘면 패스마다 함수를 나누고, beginFrame 은 공통 상태만 맡게 한다.
// 주의: GL 셰이더를 가지므로 App 에서 window_ 아래에 선언한다. draw 는 beginFrame 다음에만 부른다.
class Renderer {
public:
    Renderer();

    // 뷰포트, 지우기, 깊이 테스트, 뒷면 컬링을 설정하고 카메라와 조명 값을 셰이더에 넣는다.
    // clearColor 는 화면에 보이는 색(sRGB)이다.
    void beginFrame(const Camera& camera, const Lighting& lighting, const glm::ivec2& framebufferSize,
                    const glm::vec3& clearColor) const;
    void draw(const Mesh& mesh, const Material& material, const glm::mat4& model) const;

private:
    Shader shader_;
};
