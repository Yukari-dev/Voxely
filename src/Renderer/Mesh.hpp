#pragma once
#include "VertexBuffer.hpp"

class Mesh{
public:
    Mesh(IndexBuffer& vertexBuffer, uint32_t vertexCount);

    IndexBuffer GetVertexBuffer() { return m_VertexBuffer; }
private:
    IndexBuffer m_VertexBuffer;
    uint32_t m_VertexCount;
};