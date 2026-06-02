#include "Renderer.hpp"

Renderer::Renderer(VulkanContext &vulkanContext) : m_Context(vulkanContext) { }

void Renderer::Submit(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer){
    m_DrawQueue.push_back(&vertexBuffer);
    m_Context.RecordCommands(vertexBuffer, indexBuffer);
}

void Renderer::Render(){
    m_Context.DrawFrame();
}

