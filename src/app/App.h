// 프로그램 전체를 소유하고 메인 루프(입력 → 갱신 → 그리기 → UI)를 돌린다.
#pragma once

#include "core/FrameTimer.h"
#include "engine/platform/ImGuiLayer.h"
#include "engine/platform/Window.h"
#include "exhibition/ui/DebugPanel.h"

// 무엇을: 창, ImGui, 프레임 타이머를 멤버로 가지고 run() 에서 메인 루프를 돈다.
//         지금은 화면을 배경색으로 지우고 디버그 패널만 그린다.
// 확장:   카메라, 렌더러, 씬 같은 시스템은 멤버로 추가하고 update()/render()/drawUi() 에서 호출한다.
// 주의:   멤버는 선언 역순으로 파괴된다. GL 리소스를 가진 멤버는 반드시 window_ 보다 아래에 선언해야
//         GL 컨텍스트가 살아 있을 때 정리된다.
class App {
public:
    App();
    void run();

private:
    void handleKeys();
    void render();
    void drawUi();

    Window window_;  // 가장 먼저 생성되고 가장 나중에 파괴된다
    ImGuiLayer imgui_;
    FrameTimer timer_;

    ui::DebugPanelSettings settings_;
    bool showUi_ = true;
    bool f1WasDown_ = false;  // F1 을 누르고 있는 동안 한 번만 토글되게
};
