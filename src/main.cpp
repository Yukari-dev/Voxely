#include "Window.hpp"
#include "Vulkan/VulkanContext.hpp"
#include "Graphics/Mesh.hpp"
#include "Renderer/Renderer.hpp"
#include "Graphics/MeshFactory.hpp"
#include "Core/Camera.hpp"
#include "Core/CameraController.hpp"
#include <vector>

int main(void){
    float width = 1280.0f;
    float height = 720.0f;
    Window window(int(width), int(height), "Voxely");

    Camera camera(45.0f, width/height, 0.1f, 1000.0f);
    CameraController cameraController(camera, window.GetNativeWindow());

    VulkanContext context(window.GetNativeWindow());

    Renderer renderer(context);

    MeshFactory factory(context.GetDevice(), context.GetPhysicalDevice());

    std::vector<std::unique_ptr<Mesh>> meshes;
    int n = 10;
    for(int i = 0; i < n; i++){
        meshes.push_back(factory.CreateVoxel());
        renderer.Add(*meshes.back(), Transform{.position = {(float)i, 0, 0}});
    }
    
    float lastTime = 0.0f;
    float deltaTime = 0.0f;

    while(!window.ShouldClose()){
        float currentTime = glfwGetTime();
        deltaTime = lastTime - currentTime;
        lastTime = currentTime;

        window.PollEvents();
        cameraController.Update(deltaTime);
        renderer.UpdateCamera(camera.GetView(), camera.GetProjection());
        context.DrawFrame();
    }
    vkDeviceWaitIdle(context.GetDevice());
    return 0;
}