#pragma once
#include <memory>
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Vertex.hpp"
#include <vector>
#include "Transform.hpp"

struct RenderObject {
    VertexBuffer* vertexBuffer;
    IndexBuffer*  indexBuffer;
    Transform     transform;
};

class Mesh {
public:
    Mesh(VkDevice device, VkPhysicalDevice physicalDevice,
         const std::vector<Vertex>& vertices,
         const std::vector<uint32_t>& indices)
    {
        m_VertexBuffer = std::make_unique<VertexBuffer>(device, physicalDevice, vertices);
        m_IndexBuffer  = std::make_unique<IndexBuffer>(device, physicalDevice, indices);
    }

    VertexBuffer& GetVertexBuffer() const { return *m_VertexBuffer; }
    IndexBuffer&  GetIndexBuffer()  const { return *m_IndexBuffer;  }

private:
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer>  m_IndexBuffer;
};
