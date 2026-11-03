#include "app/App.h"

#include "core/geometry/Primitives.h"
#include "engine/gfx/GlCheck.h"

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;
constexpr const char* kWindowTitle = "Interactive 3D Virtual Exhibition";
constexpr float kFloorSizeMeters = 24.0f;

}  // namespace

App::App()
    : window_(kWindowWidth, kWindowHeight, kWindowTitle), imgui_(window_.handle()),
      floor_(makePlane(kFloorSizeMeters)), exhibits_(loadExhibits(exhibitSpecs()))
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
    // 창 크기가 바뀌어도 매 프레임 현재 크기로 뷰포트와 화면 비율을 맞춘다.
    renderer_.beginFrame(camera_, lighting_, window_.framebufferSize(), settings_.clearColor);
    renderer_.draw(floor_, floorMaterial_, glm::mat4(1.0f));
    for (const Exhibit& exhibit : exhibits_) {
        renderer_.draw(exhibit.mesh, exhibit.material, exhibit.transform.matrix());
    }
    VE_GL_CHECK();  // 어디서 났는지 모르는 GL 오류를 매 프레임 한 번 잡는다
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
