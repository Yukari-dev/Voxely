#include "Window.hpp"
#include "Vulkan/VulkanContext.hpp"
#include <vector>
#include "Graphics/Vertex.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Renderer/Renderer.hpp"

int main(void){
    float width = 1280.0f;
    float height = 720.0f;
    Window window(int(width), int(height), "Voxely");
    VulkanContext context(window.GetNativeWindow());

    Renderer renderer(context);

    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // 0
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // 1
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // 2
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}}, // 3

        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}}, // 4
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f}}, // 5
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 6
        {{-0.5f,  0.5f,  0.5f}, {0.3f, 0.3f, 0.3f}}  // 7
    };

    std::vector<uint32_t> indices = {
        // Front 
        4, 6, 5,
        4, 7, 6,

        // Back 
        0, 1, 2,
        0, 2, 3,

        // Left 
        0, 3, 7,
        0, 7, 4,

        // Right 
        1, 5, 6,
        1, 6, 2,

        // Top 
        3, 2, 6,
        3, 6, 7,

        // Bottom
        0, 4, 5,
        0, 5, 1
    };

    VertexBuffer vb(context.GetDevice(), context.GetPhysicalDevice(), vertices);
    IndexBuffer ib(context.GetDevice(), context.GetPhysicalDevice(), indices);

    auto& quad = renderer.Add(
        vb, ib, Transform{
            .position = {1, 0, 0},
            .rotation = {0, 0, 0},
            .scale = {1, 1, 1}
        }
    );

    renderer.InitDescriptors();
    renderer.BuildCommands();

    while(!window.ShouldClose()){
        window.PollEvents();
        renderer.UpdateCamera(width/height, {0, 0, 3}, {0, 0, 0});
        context.DrawFrame();
    }
    vkDeviceWaitIdle(context.GetDevice());
    return 0;
}