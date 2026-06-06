#pragma once
#include "../Window.hpp"
#include "../Vulkan/Context/VulkanContext.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Graphics/MeshFactory.hpp"
#include "../Core/Camera.hpp"
#include "../Core/CameraController.hpp"
#include "../Vulkan/Shaders/VulkanShader.hpp"
#include "../Vulkan/Textures/TextureLoader.hpp"
#include <vector>
#include <memory>

class Application{
public:
    Application(const char* title = "App", float width = 1280, float height = 720);
    virtual ~Application();

    void Run();

protected:
    virtual void OnStart() {}
    virtual void OnUpdate(float dt) {}

    void LoadTexture(const std::string& path);
    Mesh &CreateMesh(MeshType type, Transform transform = {});
    Camera &GetCamera() { return m_Camera; }

private:
    float m_Width = 1280.0f;
    float m_Height = 720.0f;

    Window m_Window;
    VulkanContext m_Context;
    Renderer m_Renderer;
    VulkanShader m_Shader;
    MeshFactory m_MeshFactory;
    std::vector<std::unique_ptr<Mesh>> m_Meshes;
    std::unique_ptr<TextureLoader> m_Texture;
    Camera m_Camera;
    CameraController m_CameraController;

};
