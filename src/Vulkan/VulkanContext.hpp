#pragma once

#include <memory>
#include <GLFW/glfw3.h>

class VulkanInstance;
class VulkanDevice;
class VulkanSwapchain;
class VulkanPipeline;
class VulkanCommands;
class VulkanSync;
class VulkanBuffer;

class VulkanContext
{
public:
    VulkanContext(GLFWwindow* window);
    ~VulkanContext();

    void DrawFrame();

private:
    GLFWwindow* m_Window;

    std::unique_ptr<VulkanInstance> m_Instance;
    std::unique_ptr<VulkanDevice> m_Device;
    std::unique_ptr<VulkanSwapchain> m_Swapchain;
    std::unique_ptr<VulkanPipeline> m_Pipeline;
    std::unique_ptr<VulkanCommands> m_Commands;
    std::unique_ptr<VulkanSync> m_Sync;
};