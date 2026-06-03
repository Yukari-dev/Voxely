cmake_minimum_required(VERSION 3.10)

project(Voxely)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(Voxely
    src/main.cpp
    src/Window.cpp
    src/Vulkan/Context/VulkanContext.cpp
    src/Vulkan/Instance/VulkanInstance.cpp
    src/Vulkan/Devices/VulkanDevice.cpp
    src/Vulkan/Pipeline/VulkanPipeline.cpp
    src/Vulkan/Descriptors/VulkanDescriptors.cpp
    src/Vulkan/Swapchain/VulkanSwapchain.cpp
    src/Vulkan/Commands/VulkanCommands.cpp
    src/Vulkan/Synchronisation/VulkanSync.cpp
    src/Core/Paths.cpp
    src/Renderer/Renderer.cpp
    src/Graphics/IndexBuffer.cpp
    src/Renderer/UniformBuffer.cpp
)

target_link_libraries(
    Voxely
    PRIVATE
    Vulkan::Vulkan
    glfw
)