package com.you.Voxely.Debug;

import com.you.Voxely.Input.Input;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;

public class DebugControls {
    private static boolean wireframe = false;

    public static void Update(){
        if(Input.IsKeyJustPressed(GLFW_KEY_F1)){
            wireframe = !wireframe;

            glPolygonMode(
                GL_FRONT_AND_BACK,
                wireframe ? GL_LINE : GL_FILL
            );
        }
    }
}
