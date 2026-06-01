#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <GLFW/glfw3.h>

class VulkanInstance;
class VulkanDevice;

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain
{
public:
    VulkanSwapchain(
        GLFWwindow* window,
        const VulkanInstance& instance,
        const VulkanDevice& device
    );

    ~VulkanSwapchain();

    VkSwapchainKHR GetSwapchain() const
    {
        return m_Swapchain;
    }

    VkFormat GetImageFormat() const
    {
        return m_ImageFormat;
    }

    VkExtent2D GetExtent() const
    {
        return m_Extent;
    }

    const std::vector<VkImage>& GetImages() const
    {
        return m_Images;
    }

    const std::vector<VkImageView>& GetImageViews() const
    {
        return m_ImageViews;
    }

private:
    void CreateSwapchain();
    void CreateImageViews();

    SwapChainSupportDetails QuerySupport(
        VkPhysicalDevice device
    );

    VkSurfaceFormatKHR ChooseSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& formats
    );

    VkPresentModeKHR ChoosePresentMode(
        const std::vector<VkPresentModeKHR>& modes
    );

    VkExtent2D ChooseExtent(
        const VkSurfaceCapabilitiesKHR& capabilities
    );

private:
    GLFWwindow* m_Window;

    VkPhysicalDevice m_PhysicalDevice;
    VkDevice m_Device;
    VkSurfaceKHR m_Surface;

    VkSwapchainKHR m_Swapchain =
        VK_NULL_HANDLE;

    VkFormat m_ImageFormat{};
    VkExtent2D m_Extent{};

    std::vector<VkImage> m_Images;
    std::vector<VkImageView> m_ImageViews;
};