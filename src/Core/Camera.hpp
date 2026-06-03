#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class Camera {
public:
    Camera(float fov, float aspect, float near, float far) : m_Fov(fov), m_Aspect(aspect), m_Near(near), m_Far(far) {}

    glm::mat4 GetView() const {
        return glm::lookAt(m_Position, m_Position + GetForward(), m_Up);
    }

    glm::mat4 GetProjection() const {
        glm::mat4 proj = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far);
        proj[1][1] *= -1;
        return proj;
    }

    glm::vec3 GetForward() const {
        return glm::normalize(glm::vec3(
            cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
            sin(glm::radians(m_Pitch)),
            sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))
        ));
    }

    glm::vec3 GetRight() const {
        return glm::normalize(glm::cross(GetForward(), m_Up));
    }

    void SetAspect(float aspect) { m_Aspect = aspect; }

    glm::vec3 m_Position = {0, 0, 3};
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;

private:
    glm::vec3 m_Up = {0, 1, 0};
    float m_Fov;
    float m_Aspect;
    float m_Near;
    float m_Far;
};