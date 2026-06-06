#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <memory>
#include <vector>
#include "VulkanShaderModule.hpp"

class VulkanShader{
public:
    VulkanShader(VkDevice device , bool isUsed);
    VulkanShader(VkDevice device);
    VulkanShader(VkDevice device, const std::string &vertexPath, const std::string &fragmentPath);

    VulkanShaderModule* GetVertexModule() const;
    VulkanShaderModule* GetFragmentModule() const;

    std::vector<VkPipelineShaderStageCreateInfo> GetShadersStageCreateInfo();

    void SetShader(std::string &path, ShaderType shaderType);
    void SetShader(std::string &vertexPath, std::string &fragmentPath);
private:
    VkDevice m_Device;
    std::unique_ptr<VulkanShaderModule> m_VertexModule;
    std::unique_ptr<VulkanShaderModule> m_FragmentModule;
};