package com.you.Voxely.Engine;

import org.joml.Vector3f;

import com.you.Voxely.Debug.DebugControls;
import com.you.Voxely.Game.VoxelyUI;
import com.you.Voxely.Input.Input;
import com.you.Voxely.Mesh.MeshCreator;
import com.you.Voxely.Renderer.Renderer;
import com.you.Voxely.Time.Time;

public abstract class VoxelyEngine {
    private Window window;
    private Renderer renderer;
    private Camera camera;
    private VoxelyUI gui;

    public void StartEngine(int width, int height, String title){
        window = new Window(width, height, title);
        camera = new Camera(window, new Vector3f(0), (float)width/(float)height);
        renderer = new Renderer(camera);
        Input.SetWindow(window.GetWindow());
        DebugControls.SetWindow(window);

        gui = new VoxelyUI();
        gui.Initialize(window.GetWindow());

        OnStart();
        Loop();
    }

    public void SpawnCube(int faceCount, Vector3f position, float[] faceColors) {
        renderer.CreateMesh(MeshCreator.MeshType.CUBE, faceCount, position, faceColors);
    }

    private void Loop(){
        while(!window.IsWindowShouldClose()){
            Time.Update();
            Input.Update();
            DebugControls.Update();
            camera.Update();

            OnUpdate();

            window.SetBackgroundColor(0, 0, 0);
            window.Clear();
            DebugControls.SetLastUsedPolygonMode();
            renderer.Draw();


            DebugControls.SetPolygonMode(false);
            gui.StartFrame();
            OnImGuiRender();;
            gui.EndFrame();

            window.SwapAndPoll();
        }
    }

    protected abstract void OnStart();
    protected abstract void OnImGuiRender();
    protected abstract void OnUpdate();
    
}
