#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <GLFW/glfw3.h>
#include "../Renderer/VertexBuffer.hpp"
#include "../Renderer/IndexBuffer.hpp"

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
    VkDevice GetDevice() const;
    VkPhysicalDevice GetPhysicalDevice() const;
    void RecordCommands(const VertexBuffer& vertexBuffer, const IndexBuffer &indexBuffer);

private:
    GLFWwindow* m_Window;

    std::unique_ptr<VulkanInstance> m_Instance;
    std::unique_ptr<VulkanDevice> m_Device;
    std::unique_ptr<VulkanSwapchain> m_Swapchain;
    std::unique_ptr<VulkanPipeline> m_Pipeline;
    std::unique_ptr<VulkanCommands> m_Commands;
    std::unique_ptr<VulkanSync> m_Sync;
};