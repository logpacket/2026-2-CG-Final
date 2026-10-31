// Dear ImGui 의 초기화·정리와 프레임 시작·끝 처리를 한곳에서 맡는다.
#pragma once

struct GLFWwindow;

// 확장: 패널은 이 클래스가 아니라 ui/ 폴더에 함수로 만든다.
// 주의: 매 프레임 beginFrame() → (패널 그리기) → endFrame() 순서로 호출한다.
//       기본 글꼴에는 한글이 없으므로 UI 문자열은 영어로 쓴다(한글은 '?' 로 보인다).
class ImGuiLayer {
public:
    explicit ImGuiLayer(GLFWwindow* window);
    ~ImGuiLayer();

    ImGuiLayer(const ImGuiLayer&) = delete;
    ImGuiLayer& operator=(const ImGuiLayer&) = delete;
    ImGuiLayer(ImGuiLayer&&) = delete;
    ImGuiLayer& operator=(ImGuiLayer&&) = delete;

    void beginFrame();
    void endFrame();

    // ImGui 가 마우스나 키보드 입력을 쓰는 중이면 true (ImGui 창 위의 마우스, 입력창에 글자 입력 중).
    // 이때는 3D 장면 쪽 입력을 무시한다.
    bool wantsMouse() const;
    bool wantsKeyboard() const;
};
