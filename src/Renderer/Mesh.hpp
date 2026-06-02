#pragma once
#include "VertexBuffer.hpp"

class Mesh{
public:
    Mesh(VertexBuffer& vertexBuffer, uint32_t vertexCount);

    VertexBuffer GetVertexBuffer() { return m_VertexBuffer; }
private:
    VertexBuffer m_VertexBuffer;
    uint32_t m_VertexCount;
};