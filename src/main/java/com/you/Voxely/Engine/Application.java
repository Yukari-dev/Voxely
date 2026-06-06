package com.you.Voxely.Engine;

import com.you.Voxely.Mesh.MeshCreator.MeshType;
import com.you.Voxely.Renderer.Renderer;

public class Application{
    private Window window;
    private Renderer renderer;

    public Application(){
        window = new Window(1280, 720, "Voxely Engine");
        renderer = new Renderer();
    }

    public void Execute(){
        renderer.CreateMesh(MeshType.CUBE);
        Loop();
    }

    public void Loop(){
        while(!window.IsWindowShouldClose()){
            window.SetBackgroundColor(0, 0, 0);
            window.Clear();
            renderer.Draw();
            window.SwapAndPoll();
        }
    }

    public void Exit(){
        window.Exit();
    }
}
