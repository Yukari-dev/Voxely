#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <stdexcept>
#include <iostream>

class VulkanInstance
{
public:
    VulkanInstance(GLFWwindow* window);
    ~VulkanInstance();

    VkInstance GetInstance() const { return m_Instance; }
    VkSurfaceKHR GetSurface() const { return m_Surface; }

private:
    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface(GLFWwindow* window);

    bool CheckValidationLayers();
    std::vector<const char*> GetRequiredExtensions();

    VkResult CreateDebugUtilsMessengerEXT(
        const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
        const VkAllocationCallbacks* allocator,
        VkDebugUtilsMessengerEXT* messenger
    );

    void DestroyDebugUtilsMessengerEXT(
        VkDebugUtilsMessengerEXT messenger,
        const VkAllocationCallbacks* allocator
    );

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
        void* userData
    );

private:
    VkInstance m_Instance = VK_NULL_HANDLE;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;

#ifdef NDEBUG
    const bool m_EnableValidationLayers = false;
#else
    const bool m_EnableValidationLayers = true;
#endif

    const std::vector<const char*> m_ValidationLayers =
    {
        "VK_LAYER_KHRONOS_validation"
    };
};