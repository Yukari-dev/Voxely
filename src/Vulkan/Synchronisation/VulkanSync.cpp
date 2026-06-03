#include "VulkanSync.hpp"

#include "../Devices/VulkanDevice.hpp"
#include "../Swapchain/VulkanSwapchain.hpp"
#include "../Commands/VulkanCommands.hpp"

#include <stdexcept>

VulkanSync::VulkanSync(
    const VulkanDevice& device,
    const VulkanSwapchain& swapchain,
    const VulkanCommands& commands
)
{
    m_Device =
        device.GetDevice();

    m_GraphicsQueue =
        device.GetGraphicsQueue();

    m_PresentQueue =
        device.GetPresentQueue();

    m_Swapchain =
        swapchain.GetSwapchain();

    m_CommandBuffers =
        commands.GetCommandBuffers();

    m_SwapchainImages =
        swapchain.GetImages();

    CreateSyncObjects();
}


VulkanSync::~VulkanSync()
{
    for(auto semaphore :
        m_ImageAvailableSemaphores)
    {
        vkDestroySemaphore(
            m_Device,
            semaphore,
            nullptr
        );
    }

    for(auto semaphore :
        m_RenderFinishedSemaphores)
    {
        vkDestroySemaphore(
            m_Device,
            semaphore,
            nullptr
        );
    }

    for(auto fence :
        m_InFlightFences)
    {
        vkDestroyFence(
            m_Device,
            fence,
            nullptr
        );
    }
}

void VulkanSync::CreateSyncObjects()
{
    m_ImageAvailableSemaphores.resize(
        MAX_FRAMES_IN_FLIGHT
    );

    m_RenderFinishedSemaphores.resize(
        m_SwapchainImages.size()
    );

    m_InFlightFences.resize(
        MAX_FRAMES_IN_FLIGHT
    );

    m_ImagesInFlight.resize(
        m_SwapchainImages.size(),
        VK_NULL_HANDLE
    );

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType =
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType =
        VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    fenceInfo.flags =
        VK_FENCE_CREATE_SIGNALED_BIT;

    for(uint32_t i = 0;
        i < MAX_FRAMES_IN_FLIGHT;
        i++)
    {
        vkCreateSemaphore(
            m_Device,
            &semaphoreInfo,
            nullptr,
            &m_ImageAvailableSemaphores[i]
        );

        vkCreateFence(
            m_Device,
            &fenceInfo,
            nullptr,
            &m_InFlightFences[i]
        );
    }

    for(size_t i = 0;
        i < m_RenderFinishedSemaphores.size();
        i++)
    {
        vkCreateSemaphore(
            m_Device,
            &semaphoreInfo,
            nullptr,
            &m_RenderFinishedSemaphores[i]
        );
    }
}

void VulkanSync::DrawFrame()
{
    vkWaitForFences(
        m_Device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX
    );

    uint32_t imageIndex;

    vkAcquireNextImageKHR(
        m_Device,
        m_Swapchain,
        UINT64_MAX,
        m_ImageAvailableSemaphores[m_CurrentFrame],
        VK_NULL_HANDLE,
        &imageIndex
    );

    if(m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(
            m_Device, 1, &m_ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX
        );
    }

    vkResetFences(
        m_Device, 1, &m_InFlightFences[m_CurrentFrame]
    );

    // Update the mapping
    m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

    VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[imageIndex] };

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers[imageIndex];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if(vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit draw.");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_Swapchain;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(m_PresentQueue, &presentInfo);

    m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}