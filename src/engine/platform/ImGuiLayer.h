// Dear ImGui 초기화/정리와 프레임 시작·끝 처리를 감싼다.
#pragma once

struct GLFWwindow;

// 무엇을: ImGui 컨텍스트와 GLFW/OpenGL3 백엔드를 만들고(생성자) 정리한다(소멸자).
//         매 프레임 beginFrame() → (패널 그리기) → endFrame() 순서로 호출한다.
// 확장:   패널 자체는 ui/ 폴더에 함수로 만든다. 이 클래스는 초기화만 담당한다.
// 주의:   기본 글꼴에는 한글이 없으므로 UI 문자열은 영어로 쓴다(한글은 '?' 로 보인다).
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

    // ImGui 창 위에 마우스/키보드가 있으면 true. 이때는 3D 장면 쪽 입력을 무시한다.
    bool wantsMouse() const;
    bool wantsKeyboard() const;
};
