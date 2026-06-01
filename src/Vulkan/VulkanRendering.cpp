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

    if(indices.graphicsFamily != indices.presentFamily){
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else{
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
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
    m_ImagesInFlight.resize(m_SwapChainImages.size(), VK_NULL_HANDLE);

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
    ) != VK_SUCCESS){
        throw std::runtime_error("Failed to Create Render Pass.");
    }

    std::cout << "Render Pass Created.\n";

}

void VulkanContext::CreateFramebuffers(){
    m_SwapChainFramebuffers.resize(
        m_SwapChainImageViews.size()
    );

    for(size_t i = 0; i < m_SwapChainImageViews.size(); i++){
        VkImageView attachmnets[] = {
            m_SwapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_RenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachmnets;
        framebufferInfo.width = m_SwapChainExtent.width;
        framebufferInfo.height = m_SwapChainExtent.height;

        framebufferInfo.layers = 1;

        if(vkCreateFramebuffer(
            m_Device, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]
        ) != VK_SUCCESS){
            throw std::runtime_error("Failed to Create Framebuffer.");
        }

    }   

    std::cout << "Created " << m_SwapChainFramebuffers.size() << " framebuffers\n";
}

void VulkanContext::CreateCommandPool(){
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
    VkCommandPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.queueFamilyIndex = indices.graphicsFamily.value();
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if(vkCreateCommandPool(
        m_Device, &createInfo, nullptr, &m_CommandPool
    ) != VK_SUCCESS){
        throw std::runtime_error("Failed to Create Command Pool.");
    }

    std::cout << "Command Pool Created.\n";
}

void VulkanContext::CreateCommandBuffers(){
    m_CommandBuffers.resize(
        m_SwapChainFramebuffers.size()
    );

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_CommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

    if(vkAllocateCommandBuffers(
        m_Device, &allocInfo, m_CommandBuffers.data()
    ) != VK_SUCCESS){
        throw std::runtime_error("Failed to allocate command buffers");
    }

    std::cout << "Allocated " << m_CommandBuffers.size() << " command buffers.\n";

    for(size_t i = 0; i < m_CommandBuffers.size(); i++){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(
            m_CommandBuffers[i], &beginInfo
        );

        VkClearValue clearColor{};
        clearColor.color = {
            {0.05, 0.08f, 0.15f, 1.0f}
        };

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_RenderPass;
        renderPassInfo.framebuffer=  m_SwapChainFramebuffers[i];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_SwapChainExtent;

        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;
        
        vkCmdBeginRenderPass(
            m_CommandBuffers[i],
            &renderPassInfo,
            VK_SUBPASS_CONTENTS_INLINE
        );

        vkCmdBindPipeline(
            m_CommandBuffers[i],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_GraphicsPipeline
        );

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;

        viewport.width = (float)m_SwapChainExtent.width;
        viewport.height = (float)m_SwapChainExtent.height;

        vkCmdSetViewport(
            m_CommandBuffers[i],
            0, 1,
            &viewport
        );

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = m_SwapChainExtent;

        vkCmdSetScissor(
            m_CommandBuffers[i],
            0, 1,
            &scissor
        );
        
        vkCmdDraw(
            m_CommandBuffers[i],
            3, 1, 0, 0
        );
        
        vkCmdEndRenderPass(m_CommandBuffers[i]);

        if(vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS){
            throw std::runtime_error("Failed to record Command Buffer.");
        }
    }

}

void VulkanContext::CreateSyncObjects(){
    m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_RenderFinishedSemaphores.resize(m_SwapChainImages.size());
    m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    for(size_t i = 0; i < m_RenderFinishedSemaphores.size(); i++)
    {
        if(vkCreateSemaphore(
            m_Device,
            &semaphoreInfo,
            nullptr,
            &m_RenderFinishedSemaphores[i]
        ) != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to create render-finished semaphore."
            );
        }
    }

    for(uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        if(
            vkCreateSemaphore(
                m_Device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]
            ) != VK_SUCCESS ||

            vkCreateFence(
                m_Device, &fenceInfo, nullptr, &m_InFlightFences[i]
            ) != VK_SUCCESS
        ){
            throw std::runtime_error(
                "Failed to create sync objects."
            );
        }
    }

    std::cout << "Sync Objects Created.\n";
}

void VulkanContext::DrawFrame(){
    vkWaitForFences(
        m_Device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX
    );

    uint32_t imageIndex;
    vkAcquireNextImageKHR(
        m_Device, m_SwapChain, UINT64_MAX,
        m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex
    );
    if (m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE){
        vkWaitForFences(
            m_Device,
            1,
            &m_ImagesInFlight[imageIndex],
            VK_TRUE,
            UINT64_MAX
        );
    }
    
    vkResetFences(
        m_Device,
        1,
        &m_InFlightFences[m_CurrentFrame]
    );

    m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

    VkSemaphore waitSemaphores[] = {
        m_ImageAvailableSemaphores[m_CurrentFrame]
    };

    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSemaphore signalSemaphore[] = {
        m_RenderFinishedSemaphores[imageIndex]
    };

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;

    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers[imageIndex];

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphore;

    if(vkQueueSubmit(
        m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]
    ) != VK_SUCCESS){
        throw std::runtime_error("Failed to submit Draw Command Buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;

    VkSemaphore presentWaitSemaphores[] = {
        m_RenderFinishedSemaphores[imageIndex]
    };

    presentInfo.pWaitSemaphores = presentWaitSemaphores;

    VkSwapchainKHR swapChains[] = {
        m_SwapChain
    };

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(m_PresentQueue, &presentInfo);

    m_CurrentFrame =(m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

