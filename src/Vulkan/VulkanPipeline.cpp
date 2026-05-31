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
    auto vertexShaderCode = ReadFile(
        (Paths::Shaders() /"triangle.vert.spv").string()
    );

    auto fragmentShaderCode = ReadFile(
        (Paths::Shaders() /"triangle.frag.spv").string()
    );

    VkShaderModule vertShaderModule = CreateShaderModule(vertexShaderCode);
    VkShaderModule fragShaderModule = CreateShaderModule(fragmentShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderCreateInfo{};
    vertShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderCreateInfo.module = vertShaderModule;
    vertShaderCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderCreateInfo{};
    fragShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderCreateInfo.module = fragShaderModule;
    fragShaderCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {
        vertShaderCreateInfo,
        fragShaderCreateInfo
    };

    VkPipelineVertexInputStateCreateInfo vertInput{};
    vertInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertInput.vertexBindingDescriptionCount = 0;
    vertInput.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewportState{};
    VkViewport viewport{};
    VkRect2D scissor{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizationState{};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.lineWidth = 1.0f;
    rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = 
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_R_BIT | 
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
        
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    if(vkCreatePipelineLayout(
        m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout
    )){ 
        throw std::runtime_error("Failed to Create Pipeline Layout.");
    }

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();


    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertInput;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizationState;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.renderPass = m_RenderPass;
    pipelineInfo.layout = m_PipelineLayout;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;

    if(vkCreateGraphicsPipelines(
        m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline 
    )){
        throw std::runtime_error("Failed to Create Graphics Pipeline");
    }

    std::cout << "Graphics Pipeline Created.\n";

    vkDestroyShaderModule(
        m_Device,
        vertShaderModule,
        nullptr
    );

    vkDestroyShaderModule(
        m_Device,
        fragShaderModule,
        nullptr
    );
}