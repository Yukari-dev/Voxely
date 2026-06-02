#include "UniformBuffer.hpp"
#include <stdexcept>
#include <cstring>

uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }
    throw std::runtime_error("Failed to find suitable memory type");
}

UniformBuffer::UniformBuffer(VulkanDevice &vulkanDevice) : m_Device(vulkanDevice.GetDevice()){
    VkDeviceSize size = sizeof(UniformBufferObject);

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkCreateBuffer(m_Device, &bufferInfo, nullptr, &m_Buffer);

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(m_Device, m_Buffer, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = FindMemoryType(vulkanDevice.GetPhysicalDevice(), memReqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vkAllocateMemory(m_Device, &allocInfo, nullptr, &m_DeviceMemory);
    vkBindBufferMemory(m_Device, m_Buffer, m_DeviceMemory, 0);
    vkMapMemory(m_Device, m_DeviceMemory, 0, size, 0, &m_Mapped);
}

UniformBuffer::~UniformBuffer() {
    vkUnmapMemory(m_Device, m_DeviceMemory);
    vkDestroyBuffer(m_Device, m_Buffer, nullptr);
    vkFreeMemory(m_Device, m_DeviceMemory, nullptr);
}

void UniformBuffer::Update(const UniformBufferObject& ubo) {
    memcpy(m_Mapped, &ubo, sizeof(ubo));
}