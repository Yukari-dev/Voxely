#include "Window.hpp"
#include "Vulkan/VulkanContext.hpp"
#include "Graphics/Mesh.hpp"
#include "Renderer/Renderer.hpp"
#include "Graphics/MeshFactory.hpp"
#include <vector>

int main(void){
    float width = 1280.0f;
    float height = 720.0f;
    Window window(int(width), int(height), "Voxely");
    VulkanContext context(window.GetNativeWindow());

    Renderer renderer(context);

    MeshFactory factory(context.GetDevice(), context.GetPhysicalDevice());

    auto cube = factory.Create(MeshType::Cube);
    renderer.Add(*cube, Transform{ .position = {0,0,0} });

    while(!window.ShouldClose()){
        window.PollEvents();
        renderer.UpdateCamera(width/height, {0, 0, 3}, {0, 0, 0});
        context.DrawFrame();
    }
    vkDeviceWaitIdle(context.GetDevice());
    return 0;
}