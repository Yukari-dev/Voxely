#pragma once
#include <vulkan/vulkan.h>
#include <string>

class TextureLoader{
public:
    TextureLoader(VkDevice device, VkPhysicalDevice pDevice, VkQueue graphicsQueue, VkCommandPool commandPool, const std::string& path);

    ~TextureLoader();


    VkImageView GetTextureImageView() const { return m_TextureView; }
    VkSampler   GetTextureSampler()   const { return m_TextureSampler; }
private:
    void CreateTextureImage(const std::string &texturePath);
    void CreateTextureView();
    void EndSingleTimeCommands(VkCommandBuffer cmd);
    void TransitionImageLayout(VkCommandBuffer cmd, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VkCommandBuffer cmd, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    VkCommandBuffer BeginSingleTimeCommands();

    VkCommandPool m_CommandPool;
    VkQueue m_GraphicsQueue;

    VkDevice m_Device;
    VkPhysicalDevice m_PhysicalDevice;
    VkImage m_Texture;
    VkImageView m_TextureView;
    VkSampler m_TextureSampler;
    VkDeviceMemory m_TextureMemory;
};
