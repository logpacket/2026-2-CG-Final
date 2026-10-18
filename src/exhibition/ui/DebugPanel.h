// Owner: B
// 디버그 패널: FPS 표시와 기본 설정(배경색, VSync, ImGui 데모 창)을 그리는 ImGui 함수.
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ui {

// 패널이 보여 주기만 하는 값. App 이 매 프레임 채워서 넘긴다.
struct DebugPanelInfo {
    float fps = 0.0f;
    float frameMs = 0.0f;
    glm::ivec2 framebufferSize{0};
};

// 패널에서 사용자가 바꾸는 값. 패널이 직접 수정하고 App 이 그 값을 읽어 쓴다.
// 확장: 새 설정(예: 렌더 모드)은 여기에 필드를 추가하고 drawDebugPanel 에서 위젯을 그린다.
struct DebugPanelSettings {
    glm::vec3 clearColor{0.10f, 0.11f, 0.13f};
    bool vsync = true;
    bool showImGuiDemo = false;  // ImGui 위젯 예제 창 (ImGui 사용법을 익힐 때 유용)
};

void drawDebugPanel(const DebugPanelInfo& info, DebugPanelSettings& settings);

}  // namespace ui
