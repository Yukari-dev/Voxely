#include "Window.hpp"
#include "Vulkan/VulkanContext.hpp"
#include <vector>
#include "Renderer/Vertex.hpp"
#include "Renderer/VertexBuffer.hpp"

int main(void){
    Window window(1280, 720, "Voxely");
    VulkanContext vulkanContext(window.GetNativeWindow());

    std::vector<Vertex> vertices = {
        {{ 0.0f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, 
        {{-0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}
    };

    VertexBuffer triangleBuffer(vulkanContext.GetDevice(), vulkanContext.GetPhysicalDevice(), vertices);

    vulkanContext.RecordCommands(triangleBuffer);

    while(!window.ShouldClose()){
        window.PollEvents();
        vulkanContext.DrawFrame();
    }
    vkDeviceWaitIdle(vulkanContext.GetDevice());
    return 0;
}