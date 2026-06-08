package com.you.Voxely.Engine;

import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.opengl.GL;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;


public class Window{
    private long window;
    private int width;
    private int height;
    private String title;

    public Window(int width, int height, String title){
        this.width = width;
        this.height = height;
        this.title = title;
        Init();
    }

    private void Init(){
        GLFWErrorCallback.createPrint(System.err).set();
        glfwInit();

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(width, height, title, 0, 0);

        glfwMakeContextCurrent(window);

        glfwSetFramebufferSizeCallback(window, (window, nWidth, nHeight) -> {
            glViewport(0, 0, nWidth, nHeight);
            this.width = nWidth;
            this.height = nHeight;
        });
        GL.createCapabilities();

        glfwSwapInterval(1);
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
    }

    public void SetBackgroundColor(float r, float g, float b){
        glClearColor(r, g, b, 1.0f);
    }

    public void Clear(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    public void SwapAndPoll(){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    public void SetWindowShouldClose(boolean should){
        glfwSetWindowShouldClose(window, should);
    }

    public boolean IsWindowShouldClose(){
        return glfwWindowShouldClose(window);
    }

    public void SetWidth(int width){
        this.width = width;
    }

    public void SetHeight(int height){
        this.height = height;
    }

    public long GetWindow(){
        return window;
    }

    public void Exit(){
        glfwTerminate();
    }

}
