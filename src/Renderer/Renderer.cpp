#include "Renderer.hpp"
#include "../Graphics/UniformBufferObject.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <algorithm>

Renderer::Renderer(VulkanContext& context)
    : m_Context(context)
    , m_UniformBuffer(context.GetUniformBuffer())
    , m_Descriptors(context.GetDescriptors())
{}

RenderObject& Renderer::Add(Mesh& mesh, Transform transform) {
    m_Objects.push_back({ &mesh.GetVertexBuffer(), &mesh.GetIndexBuffer(), transform });
    InitDescriptors();
    BuildCommands();
    return m_Objects.back();
}

RenderObject& Renderer::Add(VertexBuffer& vb, IndexBuffer& ib, Transform transform) {
    m_Objects.push_back({ &vb, &ib, transform });
    InitDescriptors();
    BuildCommands();
    return m_Objects.back();
}

void Renderer::Remove(RenderObject* obj) {
    m_Objects.erase(
        std::remove_if(m_Objects.begin(), m_Objects.end(),
            [obj](const RenderObject& o){ return &o == obj; }),
        m_Objects.end()
    );
}

void Renderer::Clear() {
    m_Objects.clear();
}

void Renderer::InitDescriptors() {
    m_Descriptors.UpdateUniformBuffer(
        m_UniformBuffer.GetBuffer(), sizeof(UniformBufferObject)
    );
}

void Renderer::BuildCommands() {
    if(m_Objects.empty()) return;
    m_Context.RecordCommands(m_Objects);
}

void Renderer::UpdateCamera(float aspectRatio, glm::vec3 eye, glm::vec3 target) {
    UniformBufferObject ubo{};
    ubo.view       = glm::lookAt(eye, target, glm::vec3(0,1,0));
    ubo.projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    ubo.projection[1][1] *= -1;
    m_UniformBuffer.Update(ubo);
}