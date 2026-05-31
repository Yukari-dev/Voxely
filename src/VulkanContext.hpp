#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>

class VulkanContext{
public:
    VulkanContext(GLFWwindow *window);
    ~VulkanContext();
private:
    void CreateInstance();
    bool CheckValidationLayers();
    void SetupDebugMessenger();
    void CreateSurface(GLFWwindow *window);
    void PickPhysicalDevice();

    std::vector<const char*> GetRequiredInstanceExtensions();

    VkInstance m_Instance;
    VkSurfaceKHR m_Surface;
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;

    VkDebugUtilsMessengerEXT m_DebugMessenger{};

    const std::vector<char const*> m_ValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifdef NDEBUG
        const bool m_EnableValidationLayers = false;
    #else
        const bool m_EnableValidationLayers = true;
    #endif
};