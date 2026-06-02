#include "VulkanInstance.hpp"
#include <cstring>

VulkanInstance::VulkanInstance(GLFWwindow* window){
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface(window);
}

VulkanInstance::~VulkanInstance(){
    if (m_Surface)
    {
        vkDestroySurfaceKHR(
            m_Instance,
            m_Surface,
            nullptr
        );
    }

    if (m_EnableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(
            m_DebugMessenger,
            nullptr
        );
    }

    if (m_Instance)
    {
        vkDestroyInstance(
            m_Instance,
            nullptr
        );
    }
}

void VulkanInstance::CreateInstance(){
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Voxely";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = GetRequiredExtensions();

    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(extensions.size());

    createInfo.ppEnabledExtensionNames =
        extensions.data();

    if (m_EnableValidationLayers)
    {
        createInfo.enabledLayerCount =
            static_cast<uint32_t>(m_ValidationLayers.size());

        createInfo.ppEnabledLayerNames =
            m_ValidationLayers.data();
    }

    if (vkCreateInstance(
        &createInfo,
        nullptr,
        &m_Instance
    ) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create Vulkan instance."
        );
    }
}

void VulkanInstance::CreateSurface(GLFWwindow* window){
    if (glfwCreateWindowSurface(
        m_Instance,
        window,
        nullptr,
        &m_Surface
    ) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create surface."
        );
    }
}

std::vector<const char*> VulkanInstance::GetRequiredExtensions(){
    uint32_t glfwExtensionCount = 0;

    const char** glfwExtensions =
        glfwGetRequiredInstanceExtensions(
            &glfwExtensionCount
        );

    std::vector<const char*> extensions(
        glfwExtensions,
        glfwExtensions + glfwExtensionCount
    );

    if (m_EnableValidationLayers)
    {
        extensions.push_back(
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        );
    }

    return extensions;
}

bool VulkanInstance::CheckValidationLayers(){
    uint32_t layerCount;

    vkEnumerateInstanceLayerProperties(
        &layerCount,
        nullptr
    );

    std::vector<VkLayerProperties> layers(
        layerCount
    );

    vkEnumerateInstanceLayerProperties(
        &layerCount,
        layers.data()
    );

    for (const char* layerName : m_ValidationLayers)
    {
        bool found = false;

        for (const auto& layer : layers)
        {
            if (strcmp(
                layerName,
                layer.layerName
            ) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            return false;
        }
    }

    return true;
}

void VulkanInstance::SetupDebugMessenger() {
    if (!m_EnableValidationLayers) return;

    if (!CheckValidationLayers()) {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
    createInfo.pUserData = nullptr; 

    if (CreateDebugUtilsMessengerEXT(&createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("Failed to set up debug messenger!");
    }
}

VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(
    const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
    const VkAllocationCallbacks* allocator,
    VkDebugUtilsMessengerEXT* messenger) 
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        m_Instance, "vkCreateDebugUtilsMessengerEXT");
    
    if (func != nullptr) {
        return func(m_Instance, createInfo, allocator, messenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanInstance::DestroyDebugUtilsMessengerEXT(
    VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks* allocator) 
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        m_Instance, "vkDestroyDebugUtilsMessengerEXT");
    
    if (func != nullptr) {
        func(m_Instance, messenger, allocator);
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData) 
{
    std::cerr << "Validation Layer: " << callbackData->pMessage << std::endl;
    return VK_FALSE; // Returning VK_FALSE denotes that the call that triggered the validation should not be aborted.
}