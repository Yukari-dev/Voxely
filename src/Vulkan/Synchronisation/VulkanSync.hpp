#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class VulkanDevice;
class VulkanSwapchain;
class VulkanCommands;

class VulkanSync
{
public:
    VulkanSync(
        const VulkanDevice& device,
        const VulkanSwapchain& swapchain,
        const VulkanCommands& commands
    );

    ~VulkanSync();

    void DrawFrame();

private:
    void CreateSyncObjects();

private:
    static constexpr uint32_t
        MAX_FRAMES_IN_FLIGHT = 2;

    VkDevice m_Device{};
    VkQueue m_GraphicsQueue{};
    VkQueue m_PresentQueue{};
    VkSwapchainKHR m_Swapchain{};

    std::vector<VkCommandBuffer>
        m_CommandBuffers;

    std::vector<VkImage>
        m_SwapchainImages;

    std::vector<VkSemaphore>
        m_ImageAvailableSemaphores;

    std::vector<VkSemaphore>
        m_RenderFinishedSemaphores;

    std::vector<VkFence>
        m_InFlightFences;

    std::vector<VkFence>
        m_ImagesInFlight;

    uint32_t m_CurrentFrame = 0;
};