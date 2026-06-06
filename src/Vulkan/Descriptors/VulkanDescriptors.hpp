#pragma once
#include <vulkan/vulkan.h>
#include "../Devices/VulkanDevice.hpp"


class VulkanDescriptors{
public:
    VulkanDescriptors(VulkanDevice& device);
    ~VulkanDescriptors();

    void CreateUniformLayout();
    void CreatePool();
    void AllocateSet();

    void UpdateUniformBuffer(VkBuffer uniformBuffer, VkDeviceSize bufferSize);
    void UpdateTexture(VkImageView imageView, VkSampler sampler);
    
    VkDescriptorSetLayout GetLayout() const;
    VkDescriptorSet GetSet() const;

private:
    VkDevice m_Device;

    VkDescriptorSetLayout m_Layout{};
    VkDescriptorPool m_Pool{};
    VkDescriptorSet m_Set{};
};
