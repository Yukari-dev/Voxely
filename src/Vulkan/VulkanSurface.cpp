#include "VulkanContext.hpp"

void VulkanContext::CreateSurface(GLFWwindow *window){
    if(glfwCreateWindowSurface(m_Instance, window, nullptr, &m_Surface) != VK_SUCCESS){
        throw std::runtime_error("Failed to create Window Surface.");
    }

    std::cout << "Window Surface Created.\n";
}
