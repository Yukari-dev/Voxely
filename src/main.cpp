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

    std::vector<std::unique_ptr<Mesh>> meshes;
    int n = 10;
    for(int i = 0; i < n; i++){
        meshes.push_back(factory.CreateVoxel());
        renderer.Add(*meshes.back(), Transform{.position = {(float)i, 0, 0}});
    }

    while(!window.ShouldClose()){
        window.PollEvents();
        renderer.UpdateCamera(width/height, {0, 5, 3}, {0, 0, 0});
        context.DrawFrame();
    }
    vkDeviceWaitIdle(context.GetDevice());
    return 0;
}