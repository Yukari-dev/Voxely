#pragma once
#include "Vertex.hpp"
#include <vector>

namespace Primitives{
    inline std::vector<Vertex> CubeVertices(){
        return {
            // front
            {{-0.5f, -0.5f,  0.5f}, {1,0,0}},
            {{ 0.5f, -0.5f,  0.5f}, {0,1,0}},
            {{ 0.5f,  0.5f,  0.5f}, {0,0,1}},
            {{-0.5f,  0.5f,  0.5f}, {1,1,0}},
            // bakc
            {{-0.5f, -0.5f, -0.5f}, {1,0,1}},
            {{ 0.5f, -0.5f, -0.5f}, {0,1,1}},
            {{ 0.5f,  0.5f, -0.5f}, {1,1,1}},
            {{-0.5f,  0.5f, -0.5f}, {0,0,0}},
        };
    }

    inline std::vector<uint32_t> CubeIndices() {
        return {
            0,2,1, 2,0,3, // front
            5,7,4, 7,5,6, // back
            4,3,0, 3,4,7, // left
            1,6,5, 6,1,2, // right
            3,6,2, 6,3,7, // top
            4,1,5, 1,4,0, // bottom
        };
    }

} // namespace name

