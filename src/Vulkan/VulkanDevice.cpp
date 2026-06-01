#include "VulkanDevice.hpp"
#include "VulkanInstance.hpp"

#include <set>
#include <stdexcept>


VulkanDevice::VulkanDevice(
    const VulkanInstance& instance
){
    m_Surface = instance.GetSurface();

    PickPhysicalDevice(instance);
    CreateLogicalDevice(instance);
}

VulkanDevice::~VulkanDevice(){
    if (m_Device)
    {
        vkDestroyDevice(
            m_Device,
            nullptr
        );
    }
}

void VulkanDevice::PickPhysicalDevice(
    const VulkanInstance& instance
){
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(
        instance.GetInstance(),
        &deviceCount,
        nullptr
    );

    if (deviceCount == 0)
    {
        throw std::runtime_error(
            "No Vulkan GPU found."
        );
    }

    std::vector<VkPhysicalDevice> devices(
        deviceCount
    );

    vkEnumeratePhysicalDevices(
        instance.GetInstance(),
        &deviceCount,
        devices.data()
    );

    for (auto device : devices)
    {
        if (IsDeviceSuitable(
            device,
            instance
        ))
        {
            m_PhysicalDevice = device;
            break;
        }
    }

    if (m_PhysicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error(
            "No suitable GPU found."
        );
    }
}


void VulkanDevice::CreateLogicalDevice(
    const VulkanInstance&
){
    QueueFamilyIndices indices =
        FindQueueFamilies(
            m_PhysicalDevice
        );

    std::set<uint32_t> uniqueFamilies =
    {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    float queuePriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo>
        queueCreateInfos;

    for (uint32_t family : uniqueFamilies)
    {
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

        queueInfo.queueFamilyIndex =
            family;

        queueInfo.queueCount = 1;

        queueInfo.pQueuePriorities =
            &queuePriority;

        queueCreateInfos.push_back(
            queueInfo
        );
    }

    VkPhysicalDeviceFeatures features{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount =
        static_cast<uint32_t>(
            queueCreateInfos.size()
        );

    createInfo.pQueueCreateInfos =
        queueCreateInfos.data();

    createInfo.pEnabledFeatures =
        &features;

    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(
            m_DeviceExtensions.size()
        );

    createInfo.ppEnabledExtensionNames =
        m_DeviceExtensions.data();

    if (vkCreateDevice(
        m_PhysicalDevice,
        &createInfo,
        nullptr,
        &m_Device
    ) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create logical device."
        );
    }

    vkGetDeviceQueue(
        m_Device,
        indices.graphicsFamily.value(),
        0,
        &m_GraphicsQueue
    );

    vkGetDeviceQueue(
        m_Device,
        indices.presentFamily.value(),
        0,
        &m_PresentQueue
    );
}


QueueFamilyIndices VulkanDevice::FindQueueFamilies(
    VkPhysicalDevice device
) const{
    QueueFamilyIndices indices;

    uint32_t count = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &count,
        nullptr
    );

    std::vector<VkQueueFamilyProperties>
        families(count);

    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &count,
        families.data()
    );

    uint32_t i = 0;

    for (const auto& family : families)
    {
        if (family.queueFlags &
            VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport =
            false;

        vkGetPhysicalDeviceSurfaceSupportKHR(
            device,
            i,
            m_Surface,
            &presentSupport
        );

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.IsComplete())
        {
            break;
        }

        ++i;
    }

    return indices;
}

bool VulkanDevice::CheckDeviceExtensionSupport(
    VkPhysicalDevice device
){
    uint32_t extensionCount;

    vkEnumerateDeviceExtensionProperties(
        device,
        nullptr,
        &extensionCount,
        nullptr
    );

    std::vector<VkExtensionProperties>
        extensions(extensionCount);

    vkEnumerateDeviceExtensionProperties(
        device,
        nullptr,
        &extensionCount,
        extensions.data()
    );

    std::set<std::string> required(
        m_DeviceExtensions.begin(),
        m_DeviceExtensions.end()
    );

    for (const auto& extension : extensions)
    {
        required.erase(
            extension.extensionName
        );
    }

    return required.empty();
}

bool VulkanDevice::IsDeviceSuitable(
    VkPhysicalDevice device,
    const VulkanInstance&
){
    QueueFamilyIndices indices =
        FindQueueFamilies(device);

    bool extensionsSupported =
        CheckDeviceExtensionSupport(
            device
        );

    return indices.IsComplete() &&
           extensionsSupported;
}


