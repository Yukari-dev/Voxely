#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

class VulkanInstance;

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool IsComplete() const
    {
        return graphicsFamily.has_value() &&
               presentFamily.has_value();
    }
};

class VulkanDevice
{
public:
    explicit VulkanDevice(
        const VulkanInstance& instance
    );

    ~VulkanDevice();

    VkPhysicalDevice GetPhysicalDevice() const
    {
        return m_PhysicalDevice;
    }

    VkDevice GetDevice() const
    {
        return m_Device;
    }

    VkQueue GetGraphicsQueue() const
    {
        return m_GraphicsQueue;
    }

    VkQueue GetPresentQueue() const
    {
        return m_PresentQueue;
    }

    QueueFamilyIndices FindQueueFamilies(
        VkPhysicalDevice device
    ) const;


private:
    void PickPhysicalDevice(
        const VulkanInstance& instance
    );

    void CreateLogicalDevice(
        const VulkanInstance& instance
    );

    bool IsDeviceSuitable(
        VkPhysicalDevice device,
        const VulkanInstance& instance
    );

    bool CheckDeviceExtensionSupport(
        VkPhysicalDevice device
    );

private:
    VkPhysicalDevice m_PhysicalDevice =
        VK_NULL_HANDLE;

    VkDevice m_Device =
        VK_NULL_HANDLE;

    VkQueue m_GraphicsQueue =
        VK_NULL_HANDLE;

    VkQueue m_PresentQueue =
        VK_NULL_HANDLE;

    VkSurfaceKHR m_Surface =
        VK_NULL_HANDLE;

    const std::vector<const char*> m_DeviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
};