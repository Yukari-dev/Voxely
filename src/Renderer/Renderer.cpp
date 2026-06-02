#include "Renderer.hpp"
#include "../Graphics/UniformBufferObject.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

Renderer::Renderer(VulkanContext &vulkanContext) : 
    m_Context(vulkanContext), 
    m_UniformBuffer(m_Context.GetUniformBuffer()), 
    m_Descriptors(m_Context.GetDescriptors()) { }

void Renderer::Submit(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer){
    m_DrawQueue.push_back(&vertexBuffer);
    // m_Context.RecordCommands(vertexBuffer, indexBuffer);
}

void Renderer::InitDescriptor() {
    m_Descriptors.UpdateUniformBuffer(m_UniformBuffer.GetBuffer(), sizeof(UniformBufferObject));
}

void Renderer::UpdateUBO(float aspectRatio) {
    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    ubo.projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    ubo.projection[1][1] *= -1;
    m_UniformBuffer.Update(ubo);
}

void Renderer::Render(){
    m_Context.DrawFrame();
}

VkBuffer UniformBuffer::GetBuffer() const { return m_Buffer; }
