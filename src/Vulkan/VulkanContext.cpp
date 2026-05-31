#include "VulkanContext.hpp"




VulkanContext::VulkanContext(GLFWwindow *window) : m_Window(window){
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface(window);
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
}

VulkanContext::~VulkanContext(){
    if(m_SwapChain)
        vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);

    if(m_Device)
        vkDestroyDevice(m_Device, nullptr);

    if(m_Surface)
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

    if(m_EnableValidationLayers)
        DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

    if(m_Instance)
        vkDestroyInstance(m_Instance, nullptr);
}


