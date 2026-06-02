#pragma once
#include <vulkan/vulkan.h>
#include "../Vulkan/VulkanContext.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include <vector>

class Renderer{
public:
    Renderer(VulkanContext &vulkanContext);
    void Submit(VertexBuffer &vertexBuffer, IndexBuffer &indexBuffer);

    void Render();
private:
    VulkanContext &m_Context;
    std::vector<VertexBuffer*> m_DrawQueue;
};

