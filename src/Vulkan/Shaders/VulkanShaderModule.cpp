#include "VulkanShaderModule.hpp"
#include <iostream>

VulkanShaderModule::VulkanShaderModule(VkDevice device, const std::string &shaderPath, ShaderType shaderType) : m_Device(device){
    m_ShaderPath = Paths::Shaders().string() + shaderPath + ".spv";
    std::cout << "Loading shader: " << m_ShaderPath << '\n';

    CreateShaderModule();
    CreateShaderStage(shaderType);
}

VulkanShaderModule::~VulkanShaderModule(){
    if(m_ShaderModule)
        vkDestroyShaderModule(m_Device, m_ShaderModule, nullptr);
}


void VulkanShaderModule::CreateShaderModule(){
    auto shaderCode = ReadShader(m_ShaderPath);

    VkShaderModuleCreateInfo moduleInfo{};
    moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleInfo.codeSize = shaderCode.size();   
    moduleInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    if(vkCreateShaderModule(
        m_Device, &moduleInfo, nullptr, &m_ShaderModule
    )){
        throw std::runtime_error("Failed to create Shader Module");
    }

}

void VulkanShaderModule::CreateShaderStage(ShaderType shaderType){
    switch(shaderType){
        case ShaderType::VERTEX:
            CreateVertexShaderStage();
            break;
        case ShaderType::FRAGMENT:
            CreateFragmentShaderStage();
            break;
    }
}

void VulkanShaderModule::CreateVertexShaderStage(){
    m_ShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_ShaderStageInfo.module = m_ShaderModule;
    m_ShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    m_ShaderStageInfo.pName = "main";
}

void VulkanShaderModule::CreateFragmentShaderStage(){
    m_ShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_ShaderStageInfo.module = m_ShaderModule;
    m_ShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    m_ShaderStageInfo.pName = "main";
}

VkShaderModule VulkanShaderModule::GetShaderModule() const{
    return m_ShaderModule;
}


VkPipelineShaderStageCreateInfo VulkanShaderModule::GetShaderStageCreateInfo() const{
    return m_ShaderStageInfo;
}

std::vector<char> VulkanShaderModule::ReadShader(const std::string &filename){
    std::ifstream file(
        filename,
        std::ios::ate |
            std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open shader.");
    }

    size_t size =
        (size_t)file.tellg();

    std::vector<char> buffer(size);

    file.seekg(0);

    file.read(
        buffer.data(),
        size);

    file.close();

    return buffer;
}