package com.you.Voxely.Input;

import static org.lwjgl.glfw.GLFW.*;

public class Input {
    private static long window;

    public static void SetWindow(long win){
        window = win;
    }
    
    public static boolean IsKeyPressed(int key){
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    public static boolean IsKeyReleased(int key){
        return glfwGetKey(window, key) == GLFW_RELEASE;
    }

}
