#include "Window.hpp"
#include "Vulkan/VulkanContext.hpp"
#include <vector>
#include "Renderer/Vertex.hpp"
#include "Renderer/VertexBuffer.hpp"
#include "Renderer/Renderer.hpp"

int main(void){
    Window window(1280, 720, "Voxely");
    VulkanContext context(window.GetNativeWindow());

    Renderer renderer(context);

    std::vector<Vertex> vertices = {
        {{ 0.0f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, 
        {{-0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}
    };

    VertexBuffer triangleBuffer(context.GetDevice(), context.GetPhysicalDevice(), vertices);
    renderer.Submit(triangleBuffer);

    while(!window.ShouldClose()){
        window.PollEvents();
        context.DrawFrame();
    }
    vkDeviceWaitIdle(context.GetDevice());
    return 0;
}