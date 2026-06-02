#include "VulkanContext.hpp"

#include "VulkanInstance.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanCommands.hpp"
#include "VulkanSync.hpp"
#include "VulkanDescriptors.hpp"
#include "../Renderer/UniformBuffer.hpp"

VulkanContext::VulkanContext(GLFWwindow* window){
    m_Instance   = std::make_unique<VulkanInstance>(window);
    m_Device     = std::make_unique<VulkanDevice>(*m_Instance);
    m_Swapchain  = std::make_unique<VulkanSwapchain>(window, *m_Instance, *m_Device);
    m_Descriptors = std::make_unique<VulkanDescriptors>(*m_Device);
    m_UniformBuffer = std::make_unique<UniformBuffer>(*m_Device);
    m_Pipeline    = std::make_unique<VulkanPipeline>(*m_Device, *m_Swapchain, m_Descriptors->GetLayout());
    m_Pipeline->SetDescriptorLayout(m_Descriptors->GetLayout());
    m_Commands   = std::make_unique<VulkanCommands>(*m_Device, *m_Swapchain, *m_Pipeline, *m_Descriptors);
    m_Sync       = std::make_unique<VulkanSync>(*m_Device, *m_Swapchain, *m_Commands);
}

VulkanContext::~VulkanContext(){
    vkDeviceWaitIdle(
        m_Device->GetDevice()
    );
}

void VulkanContext::DrawFrame(){
    m_Sync->DrawFrame();
}

void VulkanContext::RecordCommands(const VertexBuffer& vertexBuffer, const IndexBuffer &indexBuffer) {
    m_Commands->Record(vertexBuffer, indexBuffer);
}

VkDevice VulkanContext::GetDevice() const { return m_Device->GetDevice(); }
VkPhysicalDevice VulkanContext::GetPhysicalDevice() const { return m_Device->GetPhysicalDevice(); }
UniformBuffer& VulkanContext::GetUniformBuffer() const { return *m_UniformBuffer; }
VulkanDescriptors& VulkanContext::GetDescriptors() const { return *m_Descriptors; }
