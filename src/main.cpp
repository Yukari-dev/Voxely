#include "Window.hpp"
#include "VulkanContext.hpp"

int main(void){
    Window window(1280, 720, "Voxely");

    VulkanContext vulkanContext(window.GetNativeWindow());

    while(!window.ShouldClose()){
        window.PollEvents();
    }
    return 0;
}
