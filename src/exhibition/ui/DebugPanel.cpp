// Owner: B
// drawDebugPanel 구현.
#include "exhibition/ui/DebugPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace ui {

namespace {

constexpr ImVec2 kPanelPos(10.0f, 10.0f);

}  // namespace

void drawDebugPanel(const DebugPanelInfo& info, DebugPanelSettings& settings)
{
    ImGui::SetNextWindowPos(kPanelPos, ImGuiCond_FirstUseEver);
    ImGui::Begin("Debug");
    ImGui::Text("FPS: %.1f (%.2f ms)", info.fps, info.frameMs);
    ImGui::Text("Framebuffer: %d x %d", info.framebufferSize.x, info.framebufferSize.y);
    ImGui::Separator();
    ImGui::ColorEdit3("Clear color", glm::value_ptr(settings.clearColor));
    ImGui::Checkbox("VSync", &settings.vsync);
    ImGui::Checkbox("Show ImGui demo", &settings.showImGuiDemo);
    ImGui::Separator();
    ImGui::TextDisabled("F1 toggle UI | Esc quit");
    ImGui::End();

    if (settings.showImGuiDemo) {
        ImGui::ShowDemoWindow(&settings.showImGuiDemo);
    }
}

}  // namespace ui
