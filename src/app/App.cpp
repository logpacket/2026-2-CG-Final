// Owner: 코어
// App 구현: 메인 루프.
#include "app/App.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;
constexpr const char* kWindowTitle = "Interactive 3D Virtual Exhibition";

}  // namespace

App::App() : window_(kWindowWidth, kWindowHeight, kWindowTitle), imgui_(window_.handle())
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
        return;  // ImGui 입력창에 글자를 치는 중이면 단축키를 무시
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
