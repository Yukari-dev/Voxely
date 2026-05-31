#include "VulkanContext.hpp"


void VulkanContext::CreateInstance(){
    std::cout << "Validation enabled: "
          << m_EnableValidationLayers
          << '\n';
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Voxely";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Voxely Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

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

    auto extensions = GetRequiredInstanceExtensions();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if(vkCreateInstance(
        &createInfo,
        nullptr, &m_Instance
    ) != VK_SUCCESS){
        throw std::runtime_error("Failed to create Vulkan instance");
    }
    std::cout << "Vulkan instance created!\n";
}

bool VulkanContext::CheckValidationLayers(){
    uint32_t layersCount;

    vkEnumerateInstanceLayerProperties(
        &layersCount, nullptr
    );

    std::vector<VkLayerProperties> layers(layersCount);

    vkEnumerateInstanceLayerProperties(
        &layersCount,
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

std::vector<const char*> VulkanContext::GetRequiredInstanceExtensions(){
    uint32_t glfwExtensionCount = 0;

    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions+glfwExtensionCount);

    if(m_EnableValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}







