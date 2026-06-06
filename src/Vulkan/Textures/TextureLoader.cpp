#include "TextureLoader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdexcept>
#include "../Utils/VulkanUtils.hpp"
#include <iostream>

TextureLoader::TextureLoader(VkDevice device, VkPhysicalDevice pDevice, VkQueue graphicsQueue, VkCommandPool commandPool, const std::string& path) 
: m_Device(device), m_PhysicalDevice(pDevice), m_CommandPool(commandPool), m_GraphicsQueue(graphicsQueue){
    CreateTextureImage(path);
    CreateTextureView();
}

TextureLoader::~TextureLoader(){
    vkDestroySampler(m_Device, m_TextureSampler, nullptr);
    vkDestroyImageView(m_Device, m_TextureView, nullptr);
    vkDestroyImage(m_Device, m_Texture, nullptr);
    vkFreeMemory(m_Device, m_TextureMemory, nullptr);
}

void TextureLoader::CreateTextureImage(const std::string &path){
    int width, height, channels;
    stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels) throw std::runtime_error("Failed to load texture image!");
    VkDeviceSize imageSize = width * height * 4;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingMemory;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = imageSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkCreateBuffer(m_Device, &bufferInfo, nullptr, &stagingBuffer);

    VkMemoryRequirements bufMemReqs;
    vkGetBufferMemoryRequirements(m_Device, stagingBuffer, &bufMemReqs);
    VkMemoryAllocateInfo bufAllocInfo{};
    bufAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    bufAllocInfo.allocationSize = bufMemReqs.size;
    bufAllocInfo.memoryTypeIndex = FindMemoryType(m_PhysicalDevice, bufMemReqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vkAllocateMemory(m_Device, &bufAllocInfo, nullptr, &stagingMemory);
    vkBindBufferMemory(m_Device, stagingBuffer, stagingMemory, 0);

    void* data;
    vkMapMemory(m_Device, stagingMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(m_Device, stagingMemory);
    stbi_image_free(pixels);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent = { (uint32_t)width, (uint32_t)height, 1 };
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    vkCreateImage(m_Device, &imageInfo, nullptr, &m_Texture);

    VkMemoryRequirements imgMemReqs;
    vkGetImageMemoryRequirements(m_Device, m_Texture, &imgMemReqs);
    VkMemoryAllocateInfo imgAllocInfo{};
    imgAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    imgAllocInfo.allocationSize = imgMemReqs.size;
    imgAllocInfo.memoryTypeIndex = FindMemoryType(m_PhysicalDevice, imgMemReqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkAllocateMemory(m_Device, &imgAllocInfo, nullptr, &m_TextureMemory);
    vkBindImageMemory(m_Device, m_Texture, m_TextureMemory, 0);

    auto cmd = BeginSingleTimeCommands();
    TransitionImageLayout(cmd, m_Texture, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(cmd, stagingBuffer, m_Texture, (uint32_t)width, (uint32_t)height);
    TransitionImageLayout(cmd, m_Texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    EndSingleTimeCommands(cmd);

    // cleanup staging
    vkDestroyBuffer(m_Device, stagingBuffer, nullptr);
    vkFreeMemory(m_Device, stagingMemory, nullptr);
}

void TextureLoader::CreateTextureView(){
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_Texture;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    vkCreateImageView(m_Device, &viewInfo, nullptr, &m_TextureView);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    vkCreateSampler(m_Device, &samplerInfo, nullptr, &m_TextureSampler);

}

VkCommandBuffer TextureLoader::BeginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_CommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer cmd;
    vkAllocateCommandBuffers(m_Device, &allocInfo, &cmd);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &beginInfo);
    return cmd;
}

void TextureLoader::EndSingleTimeCommands(VkCommandBuffer cmd) {
    vkEndCommandBuffer(cmd);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd;

    vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_GraphicsQueue);
    vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &cmd);
}

void TextureLoader::TransitionImageLayout(VkCommandBuffer cmd, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags srcStage, dstStage;

    if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }

    vkCmdPipelineBarrier(cmd, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void TextureLoader::CopyBufferToImage(VkCommandBuffer cmd, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkBufferImageCopy region{};
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.layerCount = 1;
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(cmd, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}
