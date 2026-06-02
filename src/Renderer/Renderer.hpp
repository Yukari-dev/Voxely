#pragma once
#include <vulkan/vulkan.h>
#include "../Vulkan/VulkanContext.hpp"
#include "../Graphics/VertexBuffer.hpp"
#include "../Graphics/IndexBuffer.hpp"
#include <vector>
#include "UniformBuffer.hpp"
#include "../Vulkan/VulkanDescriptors.hpp"

class Renderer{
public:
    Renderer(VulkanContext &vulkanContext);
    void Submit(VertexBuffer &vertexBuffer, IndexBuffer &indexBuffer);

    void Render();

    void InitDescriptor();
    void UpdateUBO(float aspectRatio);
private:
    VulkanContext &m_Context;
    std::vector<VertexBuffer*> m_DrawQueue;

    UniformBuffer& m_UniformBuffer;
    VulkanDescriptors& m_Descriptors;
};

