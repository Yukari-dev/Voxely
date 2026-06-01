#include "VulkanContext.hpp"

VulkanContext::VulkanContext(GLFWwindow *window) : m_Window(window){
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface(window);
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateGraphicsPipeline();
    CreateFramebuffers();
    CreateCommandPool();
    CreateVertexBuffer();
    CreateCommandBuffers();
    CreateSyncObjects();
}

VulkanContext::~VulkanContext(){
    vkDeviceWaitIdle(m_Device);

    for(auto semaphore : m_RenderFinishedSemaphores)
    {
        vkDestroySemaphore(
            m_Device,
            semaphore,
            nullptr
        );
    }
    

    for(uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_Device,m_InFlightFences[i],nullptr);
    }

    if(m_PipelineLayout)
        vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);

    if(m_GraphicsPipeline)
        vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);

    for(auto framebuffer : m_SwapChainFramebuffers)
        vkDestroyFramebuffer(m_Device, framebuffer, nullptr);

    for(auto view : m_SwapChainImageViews)
        vkDestroyImageView(m_Device, view, nullptr);
    
    if(m_CommandPool)
        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
    
    if(m_SwapChain)
        vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
    
    if(m_VertexBuffer)
        vkDestroyBuffer(m_Device, m_VertexBuffer, nullptr);
    
    if(m_BufferMemory)
        vkFreeMemory(m_Device, m_BufferMemory, nullptr);

    if(m_RenderPass)
        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
    
    if(m_Device)
        vkDestroyDevice(m_Device, nullptr);

    if(m_Surface)
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

    if(m_EnableValidationLayers)
        DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

    if(m_Instance)
        vkDestroyInstance(m_Instance, nullptr);
}


