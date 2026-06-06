#pragma once
#include <vulkan/vulkan.h>
#include <string>

class VulkanTexture{
public:
    VulkanTexture(std::string &texturePath);
    ~VulkanTexture();

private:
    VkImage m_Texture;
};
