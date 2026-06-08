package com.you.Voxely.Input;

import static org.lwjgl.glfw.GLFW.*;

public class Input {
    private static long window;

    private static final boolean[] current = new boolean[GLFW_KEY_LAST];
    private static final boolean[] previous = new boolean[GLFW_KEY_LAST];

    public static void SetWindow(long win){
        window = win;
    }

    public static void Update(){
        for(int i = 32; i < GLFW_KEY_LAST; i++){
            previous[i] = current[i];
            current[i] = glfwGetKey(window, i) == GLFW_PRESS;
        }
    }

    public static boolean IsKeyPressed(int key){
        return current[key];
    }

    public static boolean IsKeyJustPressed(int key){
        return current[key] && !previous[key];
    }

    public static boolean IsKeyJustReleased(int key){
        return !current[key] && previous[key];
    }
}