#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class VulkanDevice;
class VulkanSwapchain;

class VulkanPipeline
{
public:
    VulkanPipeline(const VulkanDevice& device, const VulkanSwapchain& swapchain, VkDescriptorSetLayout descriptorLayout);
    ~VulkanPipeline();

    VkRenderPass GetRenderPass() const{
        return m_RenderPass;
    }

    VkPipelineLayout GetPipelineLayout() const{
        return m_PipelineLayout;
    }

    VkPipeline GetPipeline() const{
        return m_GraphicsPipeline;
    }

    void SetDescriptorLayout(VkDescriptorSetLayout layout);

private:
    void CreateRenderPass();
    void CreateGraphicsPipeline();

    std::vector<char> ReadFile(
        const std::string& filename
    );

    VkShaderModule CreateShaderModule(
        const std::vector<char>& code
    );

private:
    VkDevice m_Device;

    VkFormat m_SwapchainFormat;

    VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;

    VkDescriptorSetLayout m_DescriptorLayout;
};