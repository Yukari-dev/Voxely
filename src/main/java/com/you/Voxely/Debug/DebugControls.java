package com.you.Voxely.Debug;

import com.you.Voxely.Engine.Window;
import com.you.Voxely.Input.Input;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;

public class DebugControls {
    private static boolean wireframe = false;
    private static boolean cursorVisible = false;
    private static Window window;

    public static void SetWindow(Window win){
        window = win;
    }

    public static void Update(){
        if(Input.IsKeyJustPressed(GLFW_KEY_F1)){
            wireframe = !wireframe;
            SetPolygonMode(wireframe);
        }
        if(Input.IsKeyJustPressed(GLFW_KEY_X)){
            cursorVisible = !cursorVisible;
            window.SetCursorVisibility(cursorVisible);
        }
    }

    public static void SetPolygonMode(boolean wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }

    public static void SetLastUsedPolygonMode(){
        SetPolygonMode(wireframe);
    }
}
