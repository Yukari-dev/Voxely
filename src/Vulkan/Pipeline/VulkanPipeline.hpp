#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <memory>
#include "../Shaders/ShaderTypes.hpp"
#include "../Shaders/VulkanShader.hpp"

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

    void SetShader(std::string &path, ShaderType shaderType);
    void SetShader(std::string &vertexPath, std::string &fragmentPath);
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
    VkFormat m_DepthFormat;

    
    std::unique_ptr<VulkanShader> m_Shaders;

    VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;

    VkDescriptorSetLayout m_DescriptorLayout;
};