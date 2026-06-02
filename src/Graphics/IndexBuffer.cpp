#include "IndexBuffer.hpp"

#include <stdexcept>
#include <cstring>

IndexBuffer::IndexBuffer(
    VkDevice device,
    VkPhysicalDevice physicalDevice,
    const std::vector<uint32_t>& indices
)
    : m_Device(device),
      m_IndexCount(static_cast<uint32_t>(indices.size()))
{
    VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create index buffer.");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(
        device,
        m_Buffer,
        &memRequirements
    );

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(
        physicalDevice,
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    if (vkAllocateMemory(
        device, &allocInfo, nullptr, &m_BufferMemory
    ) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to allocate index buffer memory."
        );
    }

    vkBindBufferMemory(device, m_Buffer, m_BufferMemory, 0);

    void* data;
    vkMapMemory(device, m_BufferMemory, 0, bufferSize, 0, &data);

    memcpy(data, indices.data(), static_cast<size_t>(bufferSize));

    vkUnmapMemory(device, m_BufferMemory);
}


uint32_t IndexBuffer::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }
    throw std::runtime_error("Failed to find suitable memory type");
}

IndexBuffer::~IndexBuffer(){
    vkDestroyBuffer(m_Device, m_Buffer, nullptr);
    vkFreeMemory(m_Device, m_BufferMemory, nullptr);
}

VkBuffer IndexBuffer::GetBuffer() const { return m_Buffer; }

uint32_t IndexBuffer::GetIndexCount() const { return m_IndexCount; }

