// GLFW 창과 OpenGL 4.1 Core 컨텍스트를 만들고 GLAD 로 GL 함수 주소를 가져온다.
#pragma once

#include <glm/vec2.hpp>

#include <string>

struct GLFWwindow;

// 확장: 전체 화면 전환, 입력 콜백 같은 기능은 이 클래스에 함수로 추가한다.
// 주의: GLFW 는 프로그램에서 한 번만 초기화해야 하므로 복사와 이동을 막아 두었다.
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
    void waitEvents();
    void swapBuffers();

    void setVSync(bool enabled);
    bool isVSync() const { return vsync_; }

    // glViewport 에는 이 값을 쓴다. 픽셀 단위라서 macOS Retina 에서는 창 크기의 2배다.
    glm::ivec2 framebufferSize() const;
    bool isMinimized() const;

    GLFWwindow* handle() const { return window_; }

private:
    GLFWwindow* window_ = nullptr;
    bool vsync_ = true;
};
