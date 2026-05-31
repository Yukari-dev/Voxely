#include "VulkanContext.hpp"

SwapChainSupportDetails VulkanContext::QuerySwapChainSupport(VkPhysicalDevice device){
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device, m_Surface, &details.capabilities
    );

    uint32_t formatCount = 0;

    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device, m_Surface, &formatCount, nullptr
    );

    if(formatCount != 0){
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, m_Surface, &formatCount, details.formats.data()
        );
    }

    uint32_t presentModesCount = 0;

    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, m_Surface, &presentModesCount, nullptr
    );

    if(presentModesCount != 0){
        details.presentModes.resize(presentModesCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, m_Surface, &presentModesCount, details.presentModes.data()
        );
    }

    return details;
}


void VulkanContext::CreateSwapChain(){
    auto support = QuerySwapChainSupport(m_PhysicalDevice);

    auto extent = ChooseSwapExtent(support.capabilities);

    auto surfaceFormat = ChooseSwapSurfaceFormat(support.formats);

    auto presentMode = ChooseSwapSurfacePresentModes(support.presentModes);

    uint32_t imageCount = support.capabilities.minImageCount+1;
    
    if(support.capabilities.maxImageCount > 0 && imageCount > support.capabilities.maxImageCount){
        imageCount = support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_Surface;

    createInfo.minImageCount = imageCount;

    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;

    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;

    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

    uint32_t queueFamilyIndices[] = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
    createInfo.preTransform = support.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;

    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(
        m_Device, &createInfo, nullptr, &m_SwapChain
    )){
        throw std::runtime_error("Failed to create SwapChain.");
    }

    vkGetSwapchainImagesKHR(
        m_Device, m_SwapChain, &imageCount, nullptr
    );

    m_SwapChainImages.resize(imageCount);

    vkGetSwapchainImagesKHR(
        m_Device, m_SwapChain, &imageCount, m_SwapChainImages.data()
    );

    m_SwapChainImageFormat = surfaceFormat.format;
    m_SwapChainExtent = extent;

    std::cout
        << "Swapchain Format Selected\n";

    std::cout
        << "Swapchain Present Mode Selected\n";
    
    std::cout
        << "Swapchain Images: "
        << imageCount
        << '\n';

    std::cout
        << "Extent: "
        << extent.width
        << " x "
        << extent.height
        << '\n';
}

VkSurfaceFormatKHR VulkanContext::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats){
    for(const auto& format : formats){
        if(format.format == VK_FORMAT_B8G8R8A8_SRGB && 
           format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    return formats[0];
}


VkPresentModeKHR VulkanContext::ChooseSwapSurfacePresentModes(const std::vector<VkPresentModeKHR>& presentModes){
    for(const auto& mode : presentModes){
        if(mode == VK_PRESENT_MODE_MAILBOX_KHR){
            return mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities){
    if(capabilities.currentExtent.width != UINT32_MAX){
        return capabilities.currentExtent;
    }

    int width;
    int height;

    glfwGetFramebufferSize(
        m_Window, &width, &height
    );

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actualExtent.width = std::clamp(
        actualExtent.width,
        capabilities.minImageExtent.width,
        capabilities.maxImageExtent.width
    );

    actualExtent.height= std::clamp(
        actualExtent.height,
        capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height
    );

    return actualExtent;
}
