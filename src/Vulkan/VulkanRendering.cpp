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

void VulkanContext::CreateImageViews(){
    m_SwapChainImageViews.resize(
        m_SwapChainImages.size()
    );

    for(size_t i = 0; i < m_SwapChainImages.size(); i++){
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_SwapChainImages[i];

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_SwapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        if(vkCreateImageView(
            m_Device, &createInfo, nullptr, &m_SwapChainImageViews[i]
        ) != VK_SUCCESS){
            throw std::runtime_error("Failed to Create Image View.");
        }
    }

    std::cout << "Created " << m_SwapChainImageViews.size() << " image views.\n";

}


void VulkanContext::CreateRenderPass(){
    VkRenderPassCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = m_SwapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &colorAttachment;


    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpass;

    createInfo.dependencyCount = 1;
    createInfo.pDependencies = &dependency;

    if(vkCreateRenderPass(
        m_Device, &createInfo, nullptr, &m_RenderPass
    )){
        throw std::runtime_error("Failed to Create Render Pass.");
    }

    std::cout << "Render Pass Created.\n";

}



