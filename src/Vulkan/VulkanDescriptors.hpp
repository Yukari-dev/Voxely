#pragma once
#include <vulkan/vulkan.h>


class VulkanDescriptors{
public:
    VulkanDescriptors(VkDevice device);
    ~VulkanDescriptors();

    void CreateUniformLayout();
    void CreatePool();
    void AllocateSet();

    void UpdateUniformBuffer(VkBuffer uniformBuffer, VkDeviceSize bufferSize);
    
    VkDescriptorSetLayout GetLayout() const;
    VkDescriptorSet GetSet() const;

private:
    VkDevice m_Device;

    VkDescriptorSetLayout m_Layout{};
    VkDescriptorPool m_Pool{};
    VkDescriptorSet m_Set{};
};
