#pragma once 

#include <GLFW/glfw3.h>
#include <string>

class Window{
public:
    Window(int width, int height, const std::string &title);
    ~Window();

    bool ShouldClose() const;
    void PollEvents();

    GLFWwindow *GetNativeWindow() const;

private:
    GLFWwindow *m_Window{};
};