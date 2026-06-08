package com.you.Voxely.Engine;

import org.joml.*;
import org.joml.Math;


public class Camera {
    private Vector3f position = new Vector3f(0.0f);
    private Vector3f up = new Vector3f(0, 1, 0);
    private float yaw = -90.0f;
    private float pitch = 0.0f;
    private float fov;
    private float aspectRatio;
    private float near;
    private float far;
    private CameraController cameraController;

    public Camera(Window window, Vector3f position, float aspectRatio){
        this.position = new Vector3f(position);
        this.fov = 60.0f;
        this.aspectRatio = aspectRatio;
        this.near = 0.1f;
        this.far = 1000.f;
        cameraController = new CameraController(window, this);
    }

    public void Update(){
        cameraController.Update();
    }

    public Matrix4f GetViewMatrix(){
        return new Matrix4f().lookAt(position, position.add(GetForward(), new Vector3f()), up);
    }

    public Matrix4f GetProjectionMatrix(){
        return new Matrix4f().perspective(Math.toRadians(fov), aspectRatio, near, far);
    }

    public Vector3f GetForward(){
        return new Vector3f(
            Math.cos(Math.toRadians(yaw)) * Math.cos(Math.toRadians(pitch)),
            Math.sin(Math.toRadians(pitch)),
            Math.sin(Math.toRadians(yaw)) * Math.cos(Math.toRadians(pitch))
        ).normalize();
    }

    public Vector3f GetRight(){
        return new Vector3f(GetForward().cross(new Vector3f(up), new Vector3f())).normalize();
    }

    public Vector3f GetPosition(){
        return new Vector3f(position);
    }

    public void SetPosition(Vector3f position){
        this.position = position;
    }

    public void SetAspect(float aspectRatio) { 
        this.aspectRatio = aspectRatio; 
    }

    public void AddYaw(float amount)   { 
        yaw += amount; 
    }
    public void AddPitch(float amount) { 
        pitch = Math.clamp(pitch + amount, -89.0f, 89.0f); 
    }
}
