package com.you.Voxely.Engine;

import org.joml.Vector3f;

import com.you.Voxely.Debug.DebugControls;
import com.you.Voxely.Input.Input;
import com.you.Voxely.Mesh.MeshCreator.MeshType;
import com.you.Voxely.Renderer.Renderer;

public class Application{
    private Window window;
    private Renderer renderer;
    private Camera camera;

    public Application(){
        window = new Window(1280, 720, "Voxely Engine");
        camera = new Camera(window, new Vector3f(0.0f, 0.0f, -5.0f), 1280.f/720.f);
        renderer = new Renderer(camera);
        Input.SetWindow(window.GetWindow());
    }

    public void Execute(){
        renderer.CreateMesh(MeshType.CUBE);
        Loop();
    }

    public void Loop(){
        while(!window.IsWindowShouldClose()){
            Update();
            Render();
        }
    }

    private void Update(){
        Input.Update();
        DebugControls.Update();
        camera.Update();
    }

    private void Render(){
        window.SetBackgroundColor(0, 0, 0);
        window.Clear();
        renderer.Draw();
        window.SwapAndPoll();
    }

    public void Exit(){
        window.Exit();
    }
}
