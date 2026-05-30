cmake_minimum_required(VERSION 3.10)

project(Voxely)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(Voxely
    src/main.cpp
    src/Window.cpp
    src/VulkanContext.cpp
)

target_link_libraries(
    Voxely
    PRIVATE
    Vulkan::Vulkan
    glfw
)