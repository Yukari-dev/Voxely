#include "VulkanContext.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>

VulkanContext::VulkanContext(){
    CreateInstance();
}

VulkanContext::~VulkanContext(){
    if(m_Instance)
        vkDestroyInstance(m_Instance, nullptr);
}

void VulkanContext::CreateInstance(){
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

    if(m_EnableValidationLayers && !CheckValidationLayers()){
        throw std::runtime_error("Validation Layers not available.");
    }

    std::cout << "Validation Layers are available.\n";

    if(m_EnableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
    } else{
        createInfo.enabledLayerCount = 0;
    }

    if(vkCreateInstance(
        &createInfo,
        nullptr, &m_Instance
    ) != VK_SUCCESS){
        throw std::runtime_error("Failed to create Vulkan instance");
    }
    std::cout << "Vulkan instance created!\n";
}

bool VulkanContext::CheckValidationLayers(){
    uint32_t layerCount;

    vkEnumerateInstanceLayerProperties(
        &layerCount, nullptr
    );

    std::vector<VkLayerProperties> layers(layerCount);

    vkEnumerateInstanceLayerProperties(
        &layerCount,
        layers.data()
    );

    for(const char *layerName : m_ValidationLayers){
        bool found = false;
        for(const auto &layer : layers){
            if(strcmp(layerName, layer.layerName) == 0){
                found = true;
                break;
            }
        }
        if(!found) return false;
    }

    return true;
}