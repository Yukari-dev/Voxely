#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class VulkanDevice;
class VulkanSwapchain;
class VulkanPipeline;

class VulkanCommands
{
public:
    VulkanCommands(
        const VulkanDevice& device,
        const VulkanSwapchain& swapchain,
        const VulkanPipeline& pipeline
    );

    ~VulkanCommands();

    VkCommandPool GetCommandPool() const
    {
        return m_CommandPool;
    }

    const std::vector<VkCommandBuffer>&
    GetCommandBuffers() const
    {
        return m_CommandBuffers;
    }

private:
    void CreateFramebuffers();
    void CreateCommandPool();
    void CreateCommandBuffers();

private:
    VkDevice m_Device{};
    VkPhysicalDevice m_PhysicalDevice{};
    VkSurfaceKHR m_Surface{};

    VkExtent2D m_Extent{};

    VkRenderPass m_RenderPass{};
    VkPipeline m_GraphicsPipeline{};

    std::vector<VkImageView> m_ImageViews;

    VkCommandPool m_CommandPool =
        VK_NULL_HANDLE;

    std::vector<VkFramebuffer>
        m_Framebuffers;

    std::vector<VkCommandBuffer>
        m_CommandBuffers;
};