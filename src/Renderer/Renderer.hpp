#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "../Vulkan/Context/VulkanContext.hpp"
#include "../Graphics/VertexBuffer.hpp"
#include "../Graphics/IndexBuffer.hpp"
#include "../Graphics/Mesh.hpp"
#include "UniformBuffer.hpp"
#include "../Vulkan/Descriptors/VulkanDescriptors.hpp"

class Renderer {
public:
    Renderer(VulkanContext& context);

    RenderObject& Add(Mesh& mesh, Transform transform = {});
    RenderObject& Add(VertexBuffer& vb, IndexBuffer& ib, Transform transform = {});
    void Remove(RenderObject* obj);
    void Clear();

    void UpdateCamera(const glm::mat4& view, const glm::mat4& projection);

private:
    void InitDescriptors();
    void BuildCommands();
    VulkanContext&      m_Context;
    UniformBuffer&      m_UniformBuffer;
    VulkanDescriptors&  m_Descriptors;

    std::vector<RenderObject> m_Objects;
};