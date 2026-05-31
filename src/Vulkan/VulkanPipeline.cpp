#include "VulkanContext.hpp"
#include <filesystem>
#include "../Core/Paths.hpp"

std::vector<char> VulkanContext::ReadFile(const std::string &filename){
    std::ifstream file(
        filename,
        std::ios::ate |
        std::ios::binary
    );

    if(!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open file."
        );
    }

    std::vector<char> buffer(file.tellg());

    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

    file.close();

    return buffer;
}

VkShaderModule VulkanContext::CreateShaderModule(const std::vector<char> &code){
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    
    if(vkCreateShaderModule(
        m_Device, &createInfo, nullptr, &shaderModule
    )){
        throw std::runtime_error("Failed to Create Shader Module.");
    }

    std::cout << "Shader Module Created.\n";

    return shaderModule;
}

void VulkanContext::CreateGraphicsPipeline(){
    std::vector<char> vertexShaderCode = ReadFile(
        (Paths::Shaders() /"triangle.vert.spv").string()
    );
    VkShaderModule vertexShaderModule = CreateShaderModule(vertexShaderCode);
}