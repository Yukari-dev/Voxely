#include "VulkanSwapchain.hpp"

#include "../Instance/VulkanInstance.hpp"
#include "../Devices/VulkanDevice.hpp"

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
    CreateDepthResources();
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

    if(m_DepthImageView)
        vkDestroyImageView(m_Device, m_DepthImageView, nullptr);
    if(m_DepthImage)
        vkDestroyImage(m_Device, m_DepthImage, nullptr);
    vkFreeMemory(m_Device, m_DepthImageMemory, nullptr);
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


VkFormat VulkanSwapchain::FindDepthFormat() const{
    std::vector<VkFormat> candidates = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT
    };

    for(VkFormat format : candidates){
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(
            m_PhysicalDevice, format, &props
        );

        if(props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT){
            return format;
        }
    }
    throw std::runtime_error("No depth format found.");
}

uint32_t VulkanSwapchain::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }
    throw std::runtime_error("Failed to find suitable memory type");
}

void VulkanSwapchain::CreateDepthResources(){
    m_DepthFormat = FindDepthFormat();
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent = { m_Extent.width, m_Extent.height, 1 };
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = m_DepthFormat;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateImage(m_Device, &imageInfo, nullptr, &m_DepthImage);

    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(m_Device, m_DepthImage, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = FindMemoryType(
        memReqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    vkAllocateMemory(m_Device, &allocInfo, nullptr, &m_DepthImageMemory);
    vkBindImageMemory(m_Device, m_DepthImage, m_DepthImageMemory, 0);

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_DepthImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = m_DepthFormat;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    vkCreateImageView(m_Device, &viewInfo, nullptr, &m_DepthImageView);
}

