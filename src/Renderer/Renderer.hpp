#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "../Vulkan/VulkanContext.hpp"
#include "../Graphics/VertexBuffer.hpp"
#include "../Graphics/IndexBuffer.hpp"
#include "../Graphics/Mesh.hpp"
#include "UniformBuffer.hpp"
#include "../Vulkan/VulkanDescriptors.hpp"

class Renderer {
public:
    Renderer(VulkanContext& context);

    // retained API
    RenderObject& Add(VertexBuffer& vb, IndexBuffer& ib, Transform transform = {});
    void Remove(RenderObject* obj);
    void Clear();

    void InitDescriptors();
    void BuildCommands();
    void UpdateCamera(float aspectRatio, glm::vec3 eye, glm::vec3 target);

private:
    VulkanContext&      m_Context;
    UniformBuffer&      m_UniformBuffer;
    VulkanDescriptors&  m_Descriptors;

    std::vector<RenderObject> m_Objects;
};