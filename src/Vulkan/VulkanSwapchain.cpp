#include "VulkanSwapchain.hpp"

#include "VulkanInstance.hpp"
#include "VulkanDevice.hpp"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <stdexcept>

VulkanSwapchain::VulkanSwapchain(
    GLFWwindow* window,
    const VulkanInstance& instance,
    const VulkanDevice& device
)
    : m_Window(window)
{
    m_PhysicalDevice =
        device.GetPhysicalDevice();

    m_Device =
        device.GetDevice();

    m_Surface =
        instance.GetSurface();

    CreateSwapchain();
    CreateImageViews();
}

VulkanSwapchain::~VulkanSwapchain()
{
    for(auto imageView : m_ImageViews)
    {
        vkDestroyImageView(
            m_Device,
            imageView,
            nullptr
        );
    }

    if(m_Swapchain)
    {
        vkDestroySwapchainKHR(
            m_Device,
            m_Swapchain,
            nullptr
        );
    }
}

SwapChainSupportDetails
VulkanSwapchain::QuerySupport(
    VkPhysicalDevice device
)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device,
        m_Surface,
        &details.capabilities
    );

    uint32_t formatCount = 0;

    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device,
        m_Surface,
        &formatCount,
        nullptr
    );

    if(formatCount)
    {
        details.formats.resize(
            formatCount
        );

        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device,
            m_Surface,
            &formatCount,
            details.formats.data()
        );
    }

    uint32_t presentCount = 0;

    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device,
        m_Surface,
        &presentCount,
        nullptr
    );

    if(presentCount)
    {
        details.presentModes.resize(
            presentCount
        );

        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device,
            m_Surface,
            &presentCount,
            details.presentModes.data()
        );
    }

    return details;
}

VkSurfaceFormatKHR
VulkanSwapchain::ChooseSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& formats
)
{
    for(const auto& format : formats)
    {
        if(
            format.format ==
            VK_FORMAT_B8G8R8A8_SRGB &&
            format.colorSpace ==
            VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        )
        {
            return format;
        }
    }

    return formats[0];
}

VkPresentModeKHR
VulkanSwapchain::ChoosePresentMode(
    const std::vector<VkPresentModeKHR>& modes
)
{
    for(auto mode : modes)
    {
        if(mode ==
            VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}


VkExtent2D
VulkanSwapchain::ChooseExtent(
    const VkSurfaceCapabilitiesKHR&
        capabilities
)
{
    if(capabilities.currentExtent.width
        != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }

    int width;
    int height;

    glfwGetFramebufferSize(
        m_Window,
        &width,
        &height
    );

    VkExtent2D extent =
    {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    extent.width =
        std::clamp(
            extent.width,
            capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width
        );

    extent.height =
        std::clamp(
            extent.height,
            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height
        );

    return extent;
}

void VulkanSwapchain::CreateSwapchain()
{
    auto support =
        QuerySupport(
            m_PhysicalDevice
        );

    auto surfaceFormat =
        ChooseSurfaceFormat(
            support.formats
        );

    auto presentMode =
        ChoosePresentMode(
            support.presentModes
        );

    auto extent =
        ChooseExtent(
            support.capabilities
        );

    uint32_t imageCount =
        support.capabilities.minImageCount + 1;

    if(
        support.capabilities.maxImageCount > 0 &&
        imageCount >
        support.capabilities.maxImageCount
    )
    {
        imageCount =
            support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

    createInfo.surface =
        m_Surface;

    createInfo.minImageCount =
        imageCount;

    createInfo.imageFormat =
        surfaceFormat.format;

    createInfo.imageColorSpace =
        surfaceFormat.colorSpace;

    createInfo.imageExtent =
        extent;

    createInfo.imageArrayLayers = 1;

    createInfo.imageUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    createInfo.imageSharingMode =
        VK_SHARING_MODE_EXCLUSIVE;

    createInfo.preTransform =
        support.capabilities.currentTransform;

    createInfo.compositeAlpha =
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode =
        presentMode;

    createInfo.clipped =
        VK_TRUE;

    if(vkCreateSwapchainKHR(
        m_Device,
        &createInfo,
        nullptr,
        &m_Swapchain
    ) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create swapchain."
        );
    }

    vkGetSwapchainImagesKHR(
        m_Device,
        m_Swapchain,
        &imageCount,
        nullptr
    );

    m_Images.resize(
        imageCount
    );

    vkGetSwapchainImagesKHR(
        m_Device,
        m_Swapchain,
        &imageCount,
        m_Images.data()
    );

    m_ImageFormat =
        surfaceFormat.format;

    m_Extent =
        extent;
}

void VulkanSwapchain::CreateImageViews()
{
    m_ImageViews.resize(
        m_Images.size()
    );

    for(size_t i = 0;
        i < m_Images.size();
        i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType =
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

        createInfo.image =
            m_Images[i];

        createInfo.viewType =
            VK_IMAGE_VIEW_TYPE_2D;

        createInfo.format =
            m_ImageFormat;

        createInfo.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_COLOR_BIT;

        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if(vkCreateImageView(
            m_Device,
            &createInfo,
            nullptr,
            &m_ImageViews[i]
        ) != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create image view."
            );
        }
    }
}


