#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <fstream>
#include <format>
#include "../../Core/Paths.hpp"
#include "ShaderTypes.hpp"


class VulkanShaderModule{
public:
    VulkanShaderModule(VkDevice device, const std::string& shaderPath, ShaderType shaderType);
    ~VulkanShaderModule();

    VkShaderModule GetShaderModule() const;
    VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo() const;
private:
    void CreateShaderModule();
    void CreateShaderStage(ShaderType shaderType);
    void CreateVertexShaderStage();
    void CreateFragmentShaderStage();
    std::vector<char> ReadShader(const std::string &shaderFile);
    VkDevice m_Device;

    VkPipelineShaderStageCreateInfo m_ShaderStageInfo{};
    VkShaderModule m_ShaderModule;
    std::string m_ShaderPath;
};