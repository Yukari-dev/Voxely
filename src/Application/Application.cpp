#include "Application.hpp"

Application::Application(const char* title, float width, float height)
    : m_Width(width), m_Height(height), m_Window(width, height, title),
    m_Context(m_Window.GetNativeWindow()),
    m_Renderer(m_Context),
    m_MeshFactory(m_Context.GetDevice(), m_Context.GetPhysicalDevice()),
    m_Camera(45.0f, width/height, 0.1f, 1000.0f),
    m_CameraController(m_Camera, m_Window.GetNativeWindow()),
    m_Shader(m_Context.GetDevice(), false){}

Application::~Application() {
    vkDeviceWaitIdle(m_Context.GetDevice());
}

void Application::Run(){
    OnStart();

    float lastTime = 0;
    float deltaTime = 0;
    while(!m_Window.ShouldClose()){
        float now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
        m_Window.PollEvents();
        m_CameraController.Update(deltaTime);
        OnUpdate(deltaTime);
        m_Renderer.UpdateCamera(m_Camera.GetView(), m_Camera.GetProjection());
        m_Context.DrawFrame();
    }
}

Mesh& Application::CreateMesh(MeshType type, Transform transform) {
    m_Meshes.push_back(m_MeshFactory.Create(type));
    m_Renderer.Add(*m_Meshes.back(), transform);
    return *m_Meshes.back();
}

void Application::LoadTexture(const std::string& path) {
    m_Texture = std::make_unique<TextureLoader>(
        m_Context.GetDevice(),
        m_Context.GetPhysicalDevice(),
        m_Context.GetGraphicsQueue(),
        m_Context.GetCommandPool(),
        path
    );
    m_Context.GetDescriptors().UpdateTexture(
        m_Texture->GetTextureImageView(),
        m_Texture->GetTextureSampler()
    );
}

