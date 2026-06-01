#include "VulkanPipeline.hpp"

#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"

#include <fstream>
#include <stdexcept>
#include <array>
#include "../Core/Paths.hpp"


VulkanPipeline::VulkanPipeline(
    const VulkanDevice& device,
    const VulkanSwapchain& swapchain
){
    m_Device =
        device.GetDevice();

    m_SwapchainFormat =
        swapchain.GetImageFormat();

    CreateRenderPass();
    CreateGraphicsPipeline();
}

VulkanPipeline::~VulkanPipeline(){
    if(m_GraphicsPipeline)
    {
        vkDestroyPipeline(
            m_Device,
            m_GraphicsPipeline,
            nullptr
        );
    }

    if(m_PipelineLayout)
    {
        vkDestroyPipelineLayout(
            m_Device,
            m_PipelineLayout,
            nullptr
        );
    }

    if(m_RenderPass)
    {
        vkDestroyRenderPass(
            m_Device,
            m_RenderPass,
            nullptr
        );
    }
}

std::vector<char>
VulkanPipeline::ReadFile(
    const std::string& filename
){
    std::ifstream file(
        filename,
        std::ios::ate |
        std::ios::binary
    );

    if(!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open shader."
        );
    }

    size_t size =
        (size_t)file.tellg();

    std::vector<char> buffer(size);

    file.seekg(0);

    file.read(
        buffer.data(),
        size
    );

    file.close();

    return buffer;
}

VkShaderModule
VulkanPipeline::CreateShaderModule(
    const std::vector<char>& code
){
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    createInfo.codeSize =
        code.size();

    createInfo.pCode =
        reinterpret_cast<
            const uint32_t*
        >(code.data());

    VkShaderModule shaderModule;

    if(vkCreateShaderModule(
        m_Device,
        &createInfo,
        nullptr,
        &shaderModule
    ) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create shader module."
        );
    }

    return shaderModule;
}


void VulkanPipeline::CreateRenderPass(){
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format =
        m_SwapchainFormat;

    colorAttachment.samples =
        VK_SAMPLE_COUNT_1_BIT;

    colorAttachment.loadOp =
        VK_ATTACHMENT_LOAD_OP_CLEAR;

    colorAttachment.storeOp =
        VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp =
        VK_ATTACHMENT_LOAD_OP_DONT_CARE;

    colorAttachment.stencilStoreOp =
        VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachment.initialLayout =
        VK_IMAGE_LAYOUT_UNDEFINED;

    colorAttachment.finalLayout =
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorRef{};
    colorRef.attachment = 0;

    colorRef.layout =
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint =
        VK_PIPELINE_BIND_POINT_GRAPHICS;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments =
        &colorRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass =
        VK_SUBPASS_EXTERNAL;

    dependency.dstSubpass = 0;

    dependency.srcStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    dependency.dstStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    dependency.dstAccessMask =
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    createInfo.attachmentCount = 1;
    createInfo.pAttachments =
        &colorAttachment;

    createInfo.subpassCount = 1;
    createInfo.pSubpasses =
        &subpass;

    createInfo.dependencyCount = 1;
    createInfo.pDependencies =
        &dependency;

    if(vkCreateRenderPass(
        m_Device,
        &createInfo,
        nullptr,
        &m_RenderPass
    ) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create render pass."
        );
    }
}

void VulkanPipeline::CreateGraphicsPipeline(){
    auto vertCode = ReadFile(
        (Paths::Shaders() /"triangle.vert.spv").string()
    );

    auto fragCode = ReadFile(
        (Paths::Shaders() /"triangle.frag.spv").string()
    );

    VkShaderModule vertModule =
        CreateShaderModule(
            vertCode
        );

    VkShaderModule fragModule =
        CreateShaderModule(
            fragCode
        );

    VkPipelineShaderStageCreateInfo vertStage{};
    vertStage.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

    vertStage.stage =
        VK_SHADER_STAGE_VERTEX_BIT;

    vertStage.module =
        vertModule;

    vertStage.pName = "main";

    VkPipelineShaderStageCreateInfo fragStage{};
    fragStage.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

    fragStage.stage =
        VK_SHADER_STAGE_FRAGMENT_BIT;

    fragStage.module =
        fragModule;

    fragStage.pName = "main";

    VkPipelineShaderStageCreateInfo stages[] =
    {
        vertStage,
        fragStage
    };

    VkPipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    VkPipelineInputAssemblyStateCreateInfo assembly{};
    assembly.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

    assembly.topology =
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

    rasterizer.polygonMode =
        VK_POLYGON_MODE_FILL;

    rasterizer.lineWidth = 1.0f;

    rasterizer.cullMode =
        VK_CULL_MODE_BACK_BIT;

    rasterizer.frontFace =
        VK_FRONT_FACE_CLOCKWISE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

    multisampling.rasterizationSamples =
        VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState blendAttachment{};
    blendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo blending{};
    blending.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

    blending.attachmentCount = 1;
    blending.pAttachments =
        &blendAttachment;

    std::vector<VkDynamicState> dynamicStates =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

    dynamicState.dynamicStateCount =
        static_cast<uint32_t>(
            dynamicStates.size()
        );

    dynamicState.pDynamicStates =
        dynamicStates.data();

    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    vkCreatePipelineLayout(
        m_Device,
        &layoutInfo,
        nullptr,
        &m_PipelineLayout
    );

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType =
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = stages;

    pipelineInfo.pVertexInputState =
        &vertexInput;

    pipelineInfo.pInputAssemblyState =
        &assembly;

    pipelineInfo.pViewportState =
        &viewportState;

    pipelineInfo.pRasterizationState =
        &rasterizer;

    pipelineInfo.pMultisampleState =
        &multisampling;

    pipelineInfo.pColorBlendState =
        &blending;

    pipelineInfo.pDynamicState =
        &dynamicState;

    pipelineInfo.layout =
        m_PipelineLayout;

    pipelineInfo.renderPass =
        m_RenderPass;

    if(vkCreateGraphicsPipelines(
        m_Device,
        VK_NULL_HANDLE,
        1,
        &pipelineInfo,
        nullptr,
        &m_GraphicsPipeline
    ) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create pipeline."
        );
    }

    vkDestroyShaderModule(
        m_Device,
        vertModule,
        nullptr
    );

    vkDestroyShaderModule(
        m_Device,
        fragModule,
        nullptr
    );
}




