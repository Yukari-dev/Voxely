#include "Renderer.hpp"

Renderer::Renderer(VulkanContext &vulkanContext) : m_Context(vulkanContext) { }

void Renderer::Submit(VertexBuffer& buffer){
    m_DrawQueue.push_back(&buffer);
    m_Context.RecordCommands(buffer);
}

void Renderer::Render(){
    m_Context.DrawFrame();
}

