#include "VulkanDescriptors.hpp"
#include <stdexcept>
#include <array>

VulkanDescriptors::VulkanDescriptors(VulkanDevice& device) : m_Device(device.GetDevice()){
    CreateUniformLayout();
    CreatePool();
    AllocateSet();
}

VulkanDescriptors::~VulkanDescriptors(){
    vkDestroyDescriptorPool(m_Device, m_Pool, nullptr);
    vkDestroyDescriptorSetLayout(m_Device, m_Layout, nullptr);
}

void VulkanDescriptors::CreateUniformLayout(){
    VkDescriptorSetLayoutBinding binding{};
    binding.binding = 0;
    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.descriptorCount = 1;
    binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


    VkDescriptorSetLayoutBinding samplerBinding{};
    samplerBinding.binding = 1;
    samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerBinding.descriptorCount = 1;
    samplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { binding, samplerBinding };


    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();


    if(vkCreateDescriptorSetLayout(
        m_Device, &layoutInfo, nullptr, &m_Layout
    )){
        throw std::runtime_error("Failed to create Descriptor Set Layout.");
    }
}


void VulkanDescriptors::CreatePool(){
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 1;

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1;

    if(vkCreateDescriptorPool(
        m_Device, &poolInfo, nullptr, &m_Pool
    )){
        throw std::runtime_error("Failed to create Descriptor Pool.");
    }
}

void VulkanDescriptors::AllocateSet(){
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_Layout;
    allocInfo.descriptorPool = m_Pool;

    if(vkAllocateDescriptorSets(
        m_Device, &allocInfo, &m_Set
    )){
        throw std::runtime_error("Failed to allocate Descriptor Set.");
    }
}

void VulkanDescriptors::UpdateUniformBuffer(VkBuffer uniformBuffer, VkDeviceSize bufferSize){
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffer;
    bufferInfo.offset = 0;
    bufferInfo.range = bufferSize;

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = m_Set;
    write.dstBinding = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.descriptorCount = 1;
    write.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(
        m_Device, 1, &write, 0, nullptr
    );
}

void VulkanDescriptors::UpdateTexture(VkImageView imageView, VkSampler sampler) {
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView   = imageView;
    imageInfo.sampler     = sampler;

    VkWriteDescriptorSet write{};
    write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet          = m_Set;
    write.dstBinding      = 1;
    write.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.descriptorCount = 1;
    write.pImageInfo      = &imageInfo;

    vkUpdateDescriptorSets(m_Device, 1, &write, 0, nullptr);
}

VkDescriptorSetLayout VulkanDescriptors::GetLayout() const{
    return m_Layout;
}

VkDescriptorSet VulkanDescriptors::GetSet() const{
    return m_Set;
}