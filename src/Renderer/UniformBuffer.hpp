#pragma once
#include <vulkan/vulkan.h>
#include "../Graphics/UniformBufferObject.hpp"
#include "../Vulkan/VulkanDevice.hpp"

class UniformBuffer{
public:
    UniformBuffer(VulkanDevice &vulkanDevice);
    ~UniformBuffer();

    VkBuffer GetBuffer() const;
    void Update(const UniformBufferObject& ubo);

private:
    void *m_Mapped;
    VkDevice m_Device; 
    VkBuffer m_Buffer;
    VkDeviceMemory m_DeviceMemory;
};