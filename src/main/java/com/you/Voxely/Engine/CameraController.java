package com.you.Voxely.Engine;

import org.joml.Vector3f;

import com.you.Voxely.Input.Input;

import static org.lwjgl.glfw.GLFW.*;

public class CameraController {
    private Camera camera;
    private Window window;
    private float speed     = 5.0f;
    private float sensitivity = 0.1f;
    private double lastX    = 0;
    private double lastY    = 0;
    private boolean firstMouse = true;
    private float deltaTime = 0;
    private float lastTime  = 0;

    public CameraController(Window window, Camera camera){
        this.window = window;
        this.camera = camera;

        // glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetCursorPosCallback(window.GetWindow(), (win, xPos, yPos) -> {
            if(firstMouse){
                lastX = xPos;
                lastY = yPos;
                firstMouse = false;
            }

            double dx = (xPos - lastX) * sensitivity;
            double dy = (lastY - yPos) * sensitivity;
            lastX = xPos;
            lastY = yPos;

            camera.AddYaw((float) dx);
            camera.AddPitch((float) dy);
        });
    }

    public void Update(){
        float now   = (float) glfwGetTime();
        deltaTime   = now - lastTime;
        lastTime    = now;

        float step  = speed * deltaTime;

        if(Input.IsKeyPressed(GLFW_KEY_W))
            camera.SetPosition(camera.GetPosition().add(camera.GetForward().mul(step, new Vector3f())));
        if(Input.IsKeyPressed(GLFW_KEY_S))
            camera.SetPosition(camera.GetPosition().add(camera.GetForward().mul(-step, new Vector3f())));
        if(Input.IsKeyPressed(GLFW_KEY_D))
            camera.SetPosition(camera.GetPosition().add(camera.GetRight().mul(step, new Vector3f())));
        if(Input.IsKeyPressed(GLFW_KEY_A))
            camera.SetPosition(camera.GetPosition().add(camera.GetRight().mul(-step, new Vector3f())));
        if(Input.IsKeyPressed(GLFW_KEY_E))
            camera.SetPosition(camera.GetPosition().add(0, step, 0));
        if(Input.IsKeyPressed(GLFW_KEY_Q))
            camera.SetPosition(camera.GetPosition().add(0, -step, 0));
    
        if(glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            window.SetWindowShouldClose(true);
    }
}