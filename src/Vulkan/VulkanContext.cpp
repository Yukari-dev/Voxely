#include "VulkanContext.hpp"

#include "VulkanInstance.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanCommands.hpp"
#include "VulkanSync.hpp"

VulkanContext::VulkanContext(GLFWwindow* window){
    m_Instance =
        std::make_unique<VulkanInstance>(
            window
        );

    m_Device =
        std::make_unique<VulkanDevice>(
            *m_Instance
        );

    m_Swapchain =
        std::make_unique<VulkanSwapchain>(
            window,
            *m_Instance,
            *m_Device
        );

    m_Pipeline =
        std::make_unique<VulkanPipeline>(
            *m_Device,
            *m_Swapchain
        );

    m_Commands =
        std::make_unique<VulkanCommands>(
            *m_Device,
            *m_Swapchain,
            *m_Pipeline
        );

    m_Sync =
        std::make_unique<VulkanSync>(
            *m_Device,
            *m_Swapchain,
            *m_Commands
        );
}

VulkanContext::~VulkanContext(){
    vkDeviceWaitIdle(
        m_Device->GetDevice()
    );
}

void VulkanContext::DrawFrame(){
    m_Sync->DrawFrame();
}

void VulkanContext::RecordCommands(const VertexBuffer& mesh) {
    std::cout << "Recording!\n";
    m_Commands->Record(mesh);
}

VkDevice VulkanContext::GetDevice() const { return m_Device->GetDevice(); }
VkPhysicalDevice VulkanContext::GetPhysicalDevice() const { return m_Device->GetPhysicalDevice(); }
