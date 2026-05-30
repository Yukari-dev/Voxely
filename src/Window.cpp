#include "Window.hpp"

Window::Window(int width, int height, const std::string &title){
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_Window = glfwCreateWindow(
        width, height,
        title.c_str(), nullptr, nullptr
    );
}

Window::~Window(){
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

void Window::PollEvents(){
    glfwPollEvents();
}

GLFWwindow *Window::GetNativeWindow() const{
    return m_Window;
}