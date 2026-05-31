#include "VulkanContext.hpp"

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

    auto swapchain = QuerySwapChainSupport(m_PhysicalDevice);

    std::cout
        << "Graphics Queue Family: "
        << indices.graphicsFamily.value()
        << '\n';

    std::cout
        << "Present Queue: "
        << indices.presentFamily.value()
        << '\n';

    std::cout
        << "Formats: "
        << swapchain.formats.size()
        << '\n';

    std::cout
        << "Present Modes: "
        << swapchain.presentModes.size()
        << '\n';

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
    
    bool swapChainSatisified = false;
    if(extensionsSupported){
        auto details = QuerySwapChainSupport(device);

        swapChainSatisified = !details.formats.empty() && !details.presentModes.empty();
    }

    return indices.IsComplete() && extensionsSupported && swapChainSatisified;
}