#pragma once
#include <vulkan/vulkan.h>

class VulkanContext{
public:
    VulkanContext();
    ~VulkanContext();
private:
    VkInstance m_Instance;
};