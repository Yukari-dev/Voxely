#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>

class VulkanContext{
public:
    VulkanContext();
    ~VulkanContext();
private:
    void CreateInstance();
    bool CheckValidationLayers();
    void SetupDebugMessenger();
    std::vector<const char*> GetRequiredInstanceExtensions();

    VkInstance m_Instance;

    VkDebugUtilsMessengerEXT m_DebugMessenger{};

    const std::vector<char const*> m_ValidationLayers = {
        "VK_LAYER_THIS_DOES_NOT_EXIST"
    };

    #ifdef NDEBUG
        const bool m_EnableValidationLayers = false;
    #else
        const bool m_EnableValidationLayers = true;
    #endif
};