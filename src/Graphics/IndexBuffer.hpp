#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class IndexBuffer
{
public:
    IndexBuffer(
        VkDevice device,
        VkPhysicalDevice physicalDevice,
        const std::vector<uint32_t>& indices
    );

    ~IndexBuffer();

    VkBuffer GetBuffer() const;
    uint32_t GetIndexCount() const;

private:
    uint32_t FindMemoryType(
        VkPhysicalDevice physicalDevice,
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties
    );

private:
    VkDevice m_Device{};

    VkBuffer m_Buffer{};
    VkDeviceMemory m_BufferMemory{};

    uint32_t m_IndexCount{};
};