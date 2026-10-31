#include "engine/platform/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <stdexcept>

namespace {

constexpr int kGLMajor = 4;  // 4.1 은 macOS 가 지원하는 가장 높은 버전이다
constexpr int kGLMinor = 1;
constexpr int kDepthBits = 24;
constexpr int kStencilBits = 8;
constexpr int kMsaaSamples = 4;

void onGlfwError(int code, const char* description)
{
    std::fprintf(stderr, "[GLFW] error %d: %s\n", code, description);
}

}  // namespace

Window::Window(int width, int height, const std::string& title)
{
    glfwSetErrorCallback(onGlfwError);
    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("glfwInit failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGLMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGLMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);  // macOS 에서 필수
    glfwWindowHint(GLFW_DEPTH_BITS, kDepthBits);
    glfwWindowHint(GLFW_STENCIL_BITS, kStencilBits);
    glfwWindowHint(GLFW_SAMPLES, kMsaaSamples);

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window_ == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window. Does the GPU support OpenGL 4.1 Core?");
    }
    glfwMakeContextCurrent(window_);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
        glfwDestroyWindow(window_);
        glfwTerminate();
        throw std::runtime_error("Failed to load OpenGL functions (GLAD)");
    }
    setVSync(true);

    std::printf("[Window] OpenGL %s | %s\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)),
                reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
}

Window::~Window()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(window_) != 0;
}

void Window::requestClose()
{
    glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::waitEvents()
{
    glfwWaitEvents();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(window_);
}

void Window::setVSync(bool enabled)
{
    vsync_ = enabled;
    glfwSwapInterval(enabled ? 1 : 0);
}

glm::ivec2 Window::framebufferSize() const
{
    glm::ivec2 size(0);
    glfwGetFramebufferSize(window_, &size.x, &size.y);
    return size;
}

bool Window::isMinimized() const
{
    const glm::ivec2 size = framebufferSize();
    return size.x == 0 || size.y == 0;
}
