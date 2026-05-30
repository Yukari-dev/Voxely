#include "VulkanContext.hpp"
#include <stdexcept>
#include <iostream>

VulkanContext::VulkanContext(){
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Voxely";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Voxely Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if(vkCreateInstance(
        &createInfo,
        nullptr, &m_Instance
    ) != VK_SUCCESS){
        throw std::runtime_error("Failed to create Vulkan instance");
    }
    std::cout << "Vulkan instance created!\n";
}

VulkanContext::~VulkanContext(){

}
