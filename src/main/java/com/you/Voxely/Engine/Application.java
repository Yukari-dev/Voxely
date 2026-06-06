package com.you.Voxely.Engine;

public class Application{
    private Window window;

    public Application(){
        window = new Window(1280, 720, "Voxely Engine");
    }

    public void Execute(){
        Loop();
    }

    public void Loop(){
        while(!window.IsWindowShouldClose()){
            window.SetBackgroundColor(0, 0, 0);
            window.ClearAndSwapBuffersAndPollEvents();
        }
    }

    public void Exit(){
        window.Exit();
    }
}
