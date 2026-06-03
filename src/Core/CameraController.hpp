#pragma once
#include <GLFW/glfw3.h>
#include "Camera.hpp"

class CameraController {
public:
    CameraController(Camera& camera, GLFWwindow* window): m_Camera(camera), m_Window(window){
        glfwSetWindowUserPointer(window, this);
        glfwSetCursorPosCallback(window, MouseCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Update(float deltaTime) {
        float speed = m_Speed * deltaTime;

        if(glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
            m_Camera.m_Position -= m_Camera.GetForward() * speed;
        if(glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
            m_Camera.m_Position += m_Camera.GetForward() * speed;
        if(glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
            m_Camera.m_Position += m_Camera.GetRight() * speed;
        if(glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
            m_Camera.m_Position -= m_Camera.GetRight() * speed;
        if(glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
            m_Camera.m_Position.y += speed;
        if(glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS)
            m_Camera.m_Position.y -= speed;
        if(glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_Window, true);
    }

private:
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* self = static_cast<CameraController*>(glfwGetWindowUserPointer(window));

        if(self->m_FirstMouse) {
            self->m_LastX = xpos;
            self->m_LastY = ypos;
            self->m_FirstMouse = false;
        }

        float dx = (xpos - self->m_LastX) * self->m_Sensitivity;
        float dy = (self->m_LastY - ypos) * self->m_Sensitivity;
        self->m_LastX = xpos;
        self->m_LastY = ypos;

        self->m_Camera.m_Yaw   += dx;
        self->m_Camera.m_Pitch  = glm::clamp(self->m_Camera.m_Pitch + dy, -89.0f, 89.0f);
    }

    Camera& m_Camera;
    GLFWwindow* m_Window;
    float m_Speed       = 5.0f;
    float m_Sensitivity = 0.1f;
    bool m_FirstMouse  = true;
    double m_LastX       = 0;
    double m_LastY       = 0;
};