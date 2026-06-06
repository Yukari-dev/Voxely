#include "VulkanShader.hpp"


VulkanShader::VulkanShader(VkDevice device, bool isUsed) : m_Device(device) {}

VulkanShader::VulkanShader(VkDevice device) : m_Device(device){
    m_VertexModule = std::make_unique<VulkanShaderModule>(device, "Default.vert", ShaderType::VERTEX);
    m_FragmentModule = std::make_unique<VulkanShaderModule>(device, "Default.frag", ShaderType::FRAGMENT);
}

VulkanShader::VulkanShader(VkDevice device, const std::string &vertexPath, const std::string &fragmentPath) : m_Device(device){
    m_VertexModule = std::make_unique<VulkanShaderModule>(device, vertexPath, ShaderType::VERTEX);
    m_FragmentModule = std::make_unique<VulkanShaderModule>(device, fragmentPath, ShaderType::FRAGMENT);
}

std::vector<VkPipelineShaderStageCreateInfo> VulkanShader::GetShadersStageCreateInfo(){
    VkPipelineShaderStageCreateInfo vertexInfo = m_VertexModule->GetShaderStageCreateInfo();
    VkPipelineShaderStageCreateInfo fragmentInfo = m_FragmentModule->GetShaderStageCreateInfo();
    std::vector<VkPipelineShaderStageCreateInfo> stages{
        vertexInfo, fragmentInfo
    };
    return stages;
}


VulkanShaderModule* VulkanShader::GetVertexModule() const {
    return m_VertexModule.get();
}

VulkanShaderModule* VulkanShader::GetFragmentModule() const {
    return m_FragmentModule.get();
}