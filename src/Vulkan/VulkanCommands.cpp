#include "VulkanCommands.hpp"

#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanPipeline.hpp"

#include <stdexcept>

VulkanCommands::VulkanCommands(
    const VulkanDevice& device,
    const VulkanSwapchain& swapchain,
    const VulkanPipeline& pipeline
)
{
    m_Device =
        device.GetDevice();

    m_PhysicalDevice =
        device.GetPhysicalDevice();

    m_Extent =
        swapchain.GetExtent();

    m_ImageViews =
        swapchain.GetImageViews();

    m_RenderPass =
        pipeline.GetRenderPass();

    m_GraphicsPipeline =
        pipeline.GetPipeline();

    CreateFramebuffers();
    CreateCommandPool();
    CreateCommandBuffers();
}

VulkanCommands::~VulkanCommands(){
    for (auto framebuffer : m_Framebuffers) {
        vkDestroyFramebuffer(m_Device, framebuffer, nullptr);
    }
    if (m_CommandPool)
    {
        vkDestroyCommandPool(
            m_Device,
            m_CommandPool,
            nullptr
        );
    }
}


void VulkanCommands::CreateFramebuffers()
{
    m_Framebuffers.resize(
        m_ImageViews.size()
    );

    for(size_t i = 0;
        i < m_ImageViews.size();
        i++)
    {
        VkImageView attachments[] =
        {
            m_ImageViews[i]
        };

        VkFramebufferCreateInfo info{};
        info.sType =
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

        info.renderPass =
            m_RenderPass;

        info.attachmentCount = 1;
        info.pAttachments =
            attachments;

        info.width =
            m_Extent.width;

        info.height =
            m_Extent.height;

        info.layers = 1;

        if(vkCreateFramebuffer(
            m_Device,
            &info,
            nullptr,
            &m_Framebuffers[i]
        ) != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create framebuffer."
            );
        }
    }
}

void VulkanCommands::CreateCommandPool()
{
    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(
        m_PhysicalDevice,
        &queueFamilyCount,
        nullptr
    );

    std::vector<VkQueueFamilyProperties>
        families(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(
        m_PhysicalDevice,
        &queueFamilyCount,
        families.data()
    );

    uint32_t graphicsFamily = 0;

    for(uint32_t i = 0;
        i < queueFamilyCount;
        i++)
    {
        if(families[i].queueFlags &
            VK_QUEUE_GRAPHICS_BIT)
        {
            graphicsFamily = i;
            break;
        }
    }

    VkCommandPoolCreateInfo info{};
    info.sType =
        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

    info.queueFamilyIndex =
        graphicsFamily;

    info.flags =
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if(vkCreateCommandPool(
        m_Device,
        &info,
        nullptr,
        &m_CommandPool
    ) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create command pool."
        );
    }
}

void VulkanCommands::CreateCommandBuffers()
{
    m_CommandBuffers.resize(m_Framebuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_CommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

    if(vkAllocateCommandBuffers(m_Device, &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS){
        throw std::runtime_error("Failed to allocate command buffers.");
    }
}

void VulkanCommands::Record(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer)
{
    for(size_t i = 0; i < m_CommandBuffers.size(); i++){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo);

        VkClearValue clearColor{};
        clearColor.color = {{0.05f, 0.08f, 0.15f, 1.0f}};

        VkRenderPassBeginInfo renderInfo{};
        renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderInfo.renderPass = m_RenderPass;
        renderInfo.framebuffer = m_Framebuffers[i];
        renderInfo.renderArea.offset = {0, 0};
        renderInfo.renderArea.extent = m_Extent;
        renderInfo.clearValueCount = 1;
        renderInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_CommandBuffers[i], &renderInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

        VkViewport viewport{};
        viewport.width = (float)m_Extent.width;
        viewport.height = (float)m_Extent.height;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_CommandBuffers[i], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.extent = m_Extent;
        vkCmdSetScissor(m_CommandBuffers[i], 0, 1, &scissor);

        DrawMesh(m_CommandBuffers[i], vertexBuffer, indexBuffer);

        vkCmdEndRenderPass(m_CommandBuffers[i]);
        vkEndCommandBuffer(m_CommandBuffers[i]);
    }
}

void VulkanCommands::DrawMesh(VkCommandBuffer cmd, const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer){
    VkBuffer vertexBuffers[] = { vertexBuffer.GetBuffer() };
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(cmd, indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(cmd, indexBuffer.GetIndexCount(), 1, 0, 0, 0);
}

