#include "VulkanContext.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <set>


static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
    void *userData){
    std::cerr
        << "[Validation] "
        << callbackData->pMessage
        << '\n';
    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *createInfo,
    const VkAllocationCallbacks *allocator,
    VkDebugUtilsMessengerEXT *messenger){
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if(func){
        return func(instance, createInfo, allocator, messenger);
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks *allocator){
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func){
        func(instance, messenger, allocator);
    }
}


VulkanContext::VulkanContext(GLFWwindow *window){
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface(window);
    PickPhysicalDevice();
    CreateLogicalDevice();
}

VulkanContext::~VulkanContext(){
    if(m_Device)
        vkDestroyDevice(m_Device, nullptr);

    if(m_Surface)
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

    if(m_EnableValidationLayers)
        DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

    if(m_Instance)
        vkDestroyInstance(m_Instance, nullptr);
}

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

void VulkanContext::SetupDebugMessenger(){
    if(!m_EnableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    createInfo.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    createInfo.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    

    createInfo.pfnUserCallback = DebugCallback;

    VkResult result =
        CreateDebugUtilsMessengerEXT(
            m_Instance,
            &createInfo,
            nullptr,
            &m_DebugMessenger
        );


    if(result != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create debug messenger."
        );
    }
    std::cout << "Debug Messenger Created.\n";
}

void VulkanContext::CreateSurface(GLFWwindow *window){
    if(glfwCreateWindowSurface(m_Instance, window, nullptr, &m_Surface) != VK_SUCCESS){
        throw std::runtime_error("Failed to create Window Surface.");
    }

    std::cout << "Window Surface Created.\n";
}

void VulkanContext::PickPhysicalDevice(){
    uint32_t deviceCount;

    vkEnumeratePhysicalDevices(
        m_Instance, &deviceCount, nullptr
    );

    if(deviceCount == 0){
        throw std::runtime_error("No Vulkan GPUs found.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);

    vkEnumeratePhysicalDevices(
        m_Instance,
        &deviceCount,
        devices.data()
    );

    for(const auto& device : devices){
        if(IsDeviceSuitable(device)){
            m_PhysicalDevice = device;
            break;
        }
    }

    if(m_PhysicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("No suitable GPU found.");
    }


    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(
        m_PhysicalDevice, &props
    );

    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);


    std::cout << "Selected GPU: " << props.deviceName << '\n';
}

QueueFamilyIndices VulkanContext::FindQueueFamilies(VkPhysicalDevice device){
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, nullptr
    );

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, queueFamilies.data()
    );  

    for(uint32_t i = 0; i < queueFamilyCount; i++){
        if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
        if(presentSupport)
            indices.presentFamily = i;

        if(indices.IsComplete())
            break;

    }

    return indices;
}

bool VulkanContext::CheckDeviceExtensionSupport(VkPhysicalDevice device){
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(
        device, nullptr, &extensionCount, nullptr
    );

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
        device, nullptr, &extensionCount, availableExtensions.data()
    );

    std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

    for(const auto& extension : availableExtensions){
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void VulkanContext::CreateLogicalDevice(){
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    VkPhysicalDeviceFeatures deviceFeatures{};
    createInfo.pEnabledFeatures = &deviceFeatures;
    if(!CheckDeviceExtensionSupport(m_PhysicalDevice))
    {
        throw std::runtime_error(
            "Required device extensions not supported."
        );
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

    if(vkCreateDevice(
        m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS
    ){
        throw std::runtime_error("Error Creating Logical Device.");
    }

    vkGetDeviceQueue(
        m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue
    );

    vkGetDeviceQueue(
        m_Device, indices.presentFamily.value(), 0, &m_PresentQueue
    );

    std::cout << "Logical Device Created.\n";

}

bool VulkanContext::IsDeviceSuitable(VkPhysicalDevice device){
    QueueFamilyIndices indices = FindQueueFamilies(device);

    bool extensionsSupported = CheckDeviceExtensionSupport(device);

    return indices.IsComplete() && extensionsSupported;
}
