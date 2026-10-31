// GLFW 창과 OpenGL 4.1 Core 컨텍스트를 만들고 GLAD 로 GL 함수를 불러온다.
#pragma once

#include <glm/vec2.hpp>

#include <string>

struct GLFWwindow;

// 무엇을: GLFW 초기화 → 창 생성 → GL 컨텍스트 → GLAD 로딩 → VSync 를 한 번에 처리한다.
//         소멸자에서 창을 닫고 GLFW 를 정리한다(RAII).
// 확장:   전체 화면 전환, 입력 콜백 같은 기능은 이 클래스에 함수로 추가한다.
// 주의:   GLFW 는 프로그램에 하나만 있어야 하므로 복사도 이동도 막아 두었다.
class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    bool shouldClose() const;
    void requestClose();
    void pollEvents();
    void waitEvents();  // 이벤트가 올 때까지 잠든다 (최소화 중 CPU 절약)
    void swapBuffers();

    void setVSync(bool enabled);
    bool isVSync() const { return vsync_; }

    // 픽셀 단위 크기. glViewport 에 쓴다. (macOS Retina 에서는 창 크기의 2배)
    glm::ivec2 framebufferSize() const;
    // 창이 최소화되면 크기가 0 이 된다. 그때는 그리지 않는다.
    bool isMinimized() const;

    GLFWwindow* handle() const { return window_; }

private:
    GLFWwindow* window_ = nullptr;
    bool vsync_ = true;
};
