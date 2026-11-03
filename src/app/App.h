// 프로그램 본체: 창, ImGui, 프레임 타이머를 갖고 메인 루프를 돌린다.
#pragma once

#include "core/FrameTimer.h"
#include "engine/gfx/Mesh.h"
#include "engine/gfx/Shader.h"
#include "engine/platform/ImGuiLayer.h"
#include "engine/platform/Window.h"
#include "exhibition/ui/DebugPanel.h"

// 확장: 카메라, 렌더러, 씬 같은 시스템은 멤버로 추가하고 run() 의 각 단계에서 호출한다.
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
    Shader solidShader_;
    Mesh box_;

    ui::DebugPanelSettings settings_;
    bool showUi_ = true;
    bool f1WasDown_ = false;
};
