#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>

struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool IsComplete() const{
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class VulkanContext{
public:
    VulkanContext(GLFWwindow *window);
    ~VulkanContext();
private:
    void CreateInstance();
    bool CheckValidationLayers();
    void SetupDebugMessenger();
    void CreateSurface(GLFWwindow *window);
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    bool IsDeviceSuitable(VkPhysicalDevice device);

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    std::vector<const char*> GetRequiredInstanceExtensions();

    VkInstance m_Instance;
    VkSurfaceKHR m_Surface;
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_Device{};
    VkQueue m_GraphicsQueue{};
    VkQueue m_PresentQueue{};

    VkDebugUtilsMessengerEXT m_DebugMessenger{};

    const std::vector<char const*> m_ValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> m_DeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    #ifdef NDEBUG
        const bool m_EnableValidationLayers = false;
    #else
        const bool m_EnableValidationLayers = true;
    #endif
};