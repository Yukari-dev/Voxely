#pragma once
#include <vulkan/vulkan.h>
#include "../Graphics/UniformBufferObject.hpp"

class UniformBuffer{
public:
    UniformBuffer(VkDevice device, VkPhysicalDevice physicalDevice);

    VkBuffer GetBuffer() const;
    void Update(const UniformBufferObject& ubo);

private:
    VkBuffer m_Buffer;
    VkDeviceMemory m_DeviceMemory;
};