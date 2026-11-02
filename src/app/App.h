// 프로그램 본체: 창, ImGui, 렌더러와 전시 부품을 갖고 메인 루프를 돌린다.
#pragma once

#include "core/FrameTimer.h"
#include "engine/gfx/Material.h"
#include "engine/gfx/Mesh.h"
#include "engine/platform/ImGuiLayer.h"
#include "engine/platform/Window.h"
#include "engine/render/Light.h"
#include "engine/render/Renderer.h"
#include "engine/scene/Camera.h"
#include "exhibition/exhibits/Exhibits.h"
#include "exhibition/ui/DebugPanel.h"

#include <vector>

// 확장: 카메라 조작, 피킹 같은 시스템은 멤버로 추가하고 run() 의 각 단계에서 호출한다.
// 주의: 멤버는 선언의 역순으로 소멸한다. GL 리소스를 가진 멤버는 반드시 window_ 보다 아래에 선언해야
//       GL 컨텍스트가 살아 있을 때 정리된다.
class App {
public:
    App();
    void run();

private:
    void handleKeys();
    void render();
    void drawUi();

    Window window_;
    ImGuiLayer imgui_;
    FrameTimer timer_;
    Renderer renderer_;
    Mesh floor_;
    std::vector<Exhibit> exhibits_;

    Camera camera_;
    Lighting lighting_;
    Material floorMaterial_{{0.20f, 0.21f, 0.23f}, 0.0f, 0.9f};

    ui::DebugPanelSettings settings_;
    bool showUi_ = true;
    bool f1WasDown_ = false;
};
