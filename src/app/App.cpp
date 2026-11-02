#include "app/App.h"

#include "core/AssetPath.h"
#include "core/geometry/Primitives.h"
#include "engine/gfx/GlCheck.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;
constexpr const char* kWindowTitle = "Interactive 3D Virtual Exhibition";

// TODO(#4): Camera 와 Renderer 로 옮긴다.
constexpr float kFovYRadians = 0.785398f;  // 45도
constexpr float kNearMeters = 0.1f;
constexpr float kFarMeters = 100.0f;

}  // namespace

App::App()
    : window_(kWindowWidth, kWindowHeight, kWindowTitle), imgui_(window_.handle()),
      solidShader_(assetPath("shaders/solid.vert"), assetPath("shaders/solid.frag")),
      box_(makeBox(glm::vec3(1.0f)))
{
    settings_.vsync = window_.isVSync();
}

void App::run()
{
    while (!window_.shouldClose()) {
        window_.pollEvents();
        if (window_.isMinimized()) {
            window_.waitEvents();  // 최소화 중에는 CPU 를 쓰지 않고 기다린다
            continue;
        }

        timer_.tick(glfwGetTime());
        imgui_.beginFrame();

        handleKeys();
        render();
        drawUi();

        imgui_.endFrame();
        window_.swapBuffers();
    }
}

void App::handleKeys()
{
    if (imgui_.wantsKeyboard()) {
        return;
    }
    GLFWwindow* window = window_.handle();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        window_.requestClose();
    }
    const bool f1Down = glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS;
    if (f1Down && !f1WasDown_) {
        showUi_ = !showUi_;
    }
    f1WasDown_ = f1Down;
}

void App::render()
{
    // 창 크기가 바뀌어도 매 프레임 현재 크기로 뷰포트를 맞춘다.
    const glm::ivec2 fb = window_.framebufferSize();
    glViewport(0, 0, fb.x, fb.y);
    glClearColor(settings_.clearColor.r, settings_.clearColor.g, settings_.clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    const float aspect = static_cast<float>(fb.x) / static_cast<float>(fb.y);
    const glm::mat4 projection = glm::perspective(kFovYRadians, aspect, kNearMeters, kFarMeters);
    const glm::mat4 view =
        glm::lookAt(glm::vec3(2.5f, 2.0f, 3.5f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    solidShader_.bind();
    solidShader_.setMat4("uModelViewProjection", projection * view);
    solidShader_.setVec3("uColor", glm::vec3(0.9f, 0.55f, 0.2f));
    box_.draw();
    VE_GL_CHECK();
}

void App::drawUi()
{
    if (!showUi_) {
        return;
    }
    ui::DebugPanelInfo info;
    info.fps = timer_.fps();
    info.frameMs = timer_.frameMilliseconds();
    info.framebufferSize = window_.framebufferSize();
    ui::drawDebugPanel(info, settings_);

    if (settings_.vsync != window_.isVSync()) {
        window_.setVSync(settings_.vsync);
    }
}
