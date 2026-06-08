package com.you.Voxely.Time;

import static org.lwjgl.glfw.GLFW.glfwGetTime;

public class Time {
    private static double deltaTime;
    private static float fps;
    private static float updateCounter = 0.5f;

    private static double lastFrame;
    private static double accumulator;
    private static int frameCount;

    public static double GetDeltaTime(){
        return deltaTime;
    }

    public static float GetFPS(){
        return fps;
    }

    public static void Update(){
        double current = glfwGetTime();
        deltaTime = current - lastFrame;
        lastFrame = current;
        accumulator+=deltaTime;
        frameCount++;

        if(accumulator >= updateCounter){
            fps = (float) ((double)frameCount / accumulator);
            accumulator = 0;
            frameCount = 0;
        }
    }
}
