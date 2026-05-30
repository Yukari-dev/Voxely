#pragma once
#include <vulkan/vulkan.h>
#include <vector>

class VulkanContext{
public:
    VulkanContext();
    ~VulkanContext();
private:
    void CreateInstance();
    bool CheckValidationLayers();
    VkInstance m_Instance;

    const std::vector<char const*> m_ValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifdef NDEBUG
        const bool m_EnableValidationLayers = false;
    #else
        const bool m_EnableValidationLayers = false;
    #endif
};