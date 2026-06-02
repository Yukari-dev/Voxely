#include "Window.hpp"
#include "Vulkan/VulkanContext.hpp"
#include <vector>
#include "Graphics/Vertex.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Renderer/Renderer.hpp"

int main(void){
    Window window(1280, 720, "Voxely");
    VulkanContext context(window.GetNativeWindow());

    Renderer renderer(context);

    std::vector<Vertex> vertices = {
        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // 0
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // 1
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // 2
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}, // 3
    };

    std::vector<uint32_t> indices = {
        0, 1, 2,
        3, 0, 2
    };

    VertexBuffer triangleBuffer(context.GetDevice(), context.GetPhysicalDevice(), vertices);
    IndexBuffer triangleIndices(context.GetDevice(), context.GetPhysicalDevice(), indices);
    renderer.Submit(triangleBuffer, triangleIndices);

    while(!window.ShouldClose()){
        window.PollEvents();
        context.DrawFrame();
    }
    vkDeviceWaitIdle(context.GetDevice());
    return 0;
}