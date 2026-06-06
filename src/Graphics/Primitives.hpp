#pragma once
#include "Vertex.hpp"
#include <vector>
#include <array>
#include <glm/glm.hpp>

namespace Primitives {

    struct FaceDef {
        glm::vec3 corners[4];
        glm::vec3 normal;
        glm::vec2 uvs[4];
    };

    inline std::array<FaceDef, 6> CubeFaces() {
        return {{
            // front
            {
                {{ -0.5f, -0.5f,  0.5f },
                 {  0.5f, -0.5f,  0.5f },
                 {  0.5f,  0.5f,  0.5f },
                 { -0.5f,  0.5f,  0.5f }},
                { 0, 0, 1 },
                {{ 0,0 }, { 1,0 }, { 1,1 }, { 0,1 }}
            },
            // back
            {
                {{  0.5f, -0.5f, -0.5f },
                 { -0.5f, -0.5f, -0.5f },
                 { -0.5f,  0.5f, -0.5f },
                 {  0.5f,  0.5f, -0.5f }},
                { 0, 0, -1 },
                {{ 0,0 }, { 1,0 }, { 1,1 }, { 0,1 }}
            },
            // right
            {
                {{  0.5f, -0.5f,  0.5f },
                 {  0.5f, -0.5f, -0.5f },
                 {  0.5f,  0.5f, -0.5f },
                 {  0.5f,  0.5f,  0.5f }},
                { 1, 0, 0 },
                {{ 0,0 }, { 1,0 }, { 1,1 }, { 0,1 }}
            },
            // left
            {
                {{ -0.5f, -0.5f, -0.5f },
                 { -0.5f, -0.5f,  0.5f },
                 { -0.5f,  0.5f,  0.5f },
                 { -0.5f,  0.5f, -0.5f }},
                { -1, 0, 0 },
                {{ 0,0 }, { 1,0 }, { 1,1 }, { 0,1 }}
            },
            // top
            {
                {{ -0.5f,  0.5f,  0.5f },
                 {  0.5f,  0.5f,  0.5f },
                 {  0.5f,  0.5f, -0.5f },
                 { -0.5f,  0.5f, -0.5f }},
                { 0, 1, 0 },
                {{ 0,0 }, { 1,0 }, { 1,1 }, { 0,1 }}
            },
            // bottom
            {
                {{ -0.5f, -0.5f, -0.5f },
                 {  0.5f, -0.5f, -0.5f },
                 {  0.5f, -0.5f,  0.5f },
                 { -0.5f, -0.5f,  0.5f }},
                { 0, -1, 0 },
                {{ 0,0 }, { 1,0 }, { 1,1 }, { 0,1 }}
            },
        }};
    }

    inline void BuildMesh(
        const std::array<FaceDef, 6>& faces,
        std::vector<Vertex>& outVertices,
        std::vector<uint32_t>& outIndices,
        glm::vec3 offset = {0,0,0})
    {
        for(auto& face : faces) {
            uint32_t base = static_cast<uint32_t>(outVertices.size());
            for(int i = 0; i < 4; i++) {
                outVertices.push_back({
                    face.corners[i] + offset,
                    face.normal,
                    face.uvs[i]
                });
            }
            outIndices.insert(outIndices.end(), {
                base+0, base+2, base+1,
                base+2, base+0, base+3
            });
        }
    }

    inline void CubeMesh(
        std::vector<Vertex>& outVertices,
        std::vector<uint32_t>& outIndices,
        glm::vec3 offset = {0,0,0})
    {
        BuildMesh(CubeFaces(), outVertices, outIndices, offset);
    }

} // namespace Primitives