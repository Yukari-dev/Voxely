cmake_minimum_required(VERSION 3.10)

project(Voxely)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(Voxely
    src/main.cpp
    src/Window.cpp
    src/Vulkan/VulkanContext.cpp
    src/Vulkan/VulkanInstance.cpp
    src/Vulkan/VulkanDevice.cpp
    src/Vulkan/VulkanPipeline.cpp
    src/Core/Paths.cpp
    src/Vulkan/VulkanSwapchain.cpp
    src/Vulkan/VulkanCommands.cpp
    src/Vulkan/VulkanSync.cpp
    src/Renderer/Renderer.cpp
)

target_link_libraries(
    Voxely
    PRIVATE
    Vulkan::Vulkan
    glfw
)