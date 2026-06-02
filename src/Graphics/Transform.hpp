#pragma once
#include<glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

struct Transform {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale    = glm::vec3(1.0f);

    glm::mat4 GetMatrix() const {
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, position);
        m = glm::rotate(m, glm::radians(rotation.x), {1,0,0});
        m = glm::rotate(m, glm::radians(rotation.y), {0,1,0});
        m = glm::rotate(m, glm::radians(rotation.z), {0,0,1});
        m = glm::scale(m, scale);
        return m;
    }
};