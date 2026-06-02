#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <cstring>
#include "Vertex.hpp"

class VertexBuffer {
public:
    VertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, const std::vector<Vertex>& vertices) 
        : m_Device(device) {
        
        m_VertexCount = static_cast<uint32_t>(vertices.size());
        VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_Device, &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS)
            throw std::runtime_error("Failed to create vertex buffer");

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_Device, m_Buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &m_BufferMemory) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate vertex buffer memory");

        vkBindBufferMemory(m_Device, m_Buffer, m_BufferMemory, 0);

        void* data;
        vkMapMemory(m_Device, m_BufferMemory, 0, bufferSize, 0, &data);
        std::memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(m_Device, m_BufferMemory);
    }

    ~VertexBuffer() {
        vkDestroyBuffer(m_Device, m_Buffer, nullptr);
        vkFreeMemory(m_Device, m_BufferMemory, nullptr);
    }

    VkBuffer GetBuffer() const { return m_Buffer; }
    uint32_t GetVertexCount() const { return m_VertexCount; }

private:
    uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }
        throw std::runtime_error("Failed to find suitable memory type");
    }

    VkDevice m_Device;
    VkBuffer m_Buffer;
    VkDeviceMemory m_BufferMemory;
    uint32_t m_VertexCount;
};