#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "../Graphics/VertexBuffer.hpp"
#include "../Graphics/IndexBuffer.hpp"

class VulkanDevice;
class VulkanSwapchain;
class VulkanPipeline;
class VulkanDescriptors;

class VulkanCommands
{
public:
    VulkanCommands(
        const VulkanDevice& device,
        const VulkanSwapchain& swapchain,
        const VulkanPipeline& pipeline,
        const VulkanDescriptors& descriptors
    );

    ~VulkanCommands();
    void Record(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer);

    void DrawMesh(VkCommandBuffer cmd, const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer);

    VkCommandPool GetCommandPool() const{
        return m_CommandPool;
    }

    const std::vector<VkCommandBuffer>&
    GetCommandBuffers() const{
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

    VkCommandPool m_CommandPool =VK_NULL_HANDLE;
    std::vector<VkFramebuffer> m_Framebuffers;
    std::vector<VkCommandBuffer> m_CommandBuffers;

    VkDescriptorSetLayout m_DescriptorLayout{};
    VkDescriptorSet m_DescriptorSet{};
    VkPipelineLayout m_PipelineLayout;

};