package com.you.Voxely.Game;

import org.joml.Vector3f;

import com.you.Voxely.Debug.DebugControls;
import com.you.Voxely.Engine.VoxelyEngine;
import com.you.Voxely.Time.Time;

import imgui.ImGui;

public class Voxely extends VoxelyEngine {

    @Override
    protected void OnStart() {
        int chunkWidth = 8;
        int chunkDepth = 8;
        int chunkHeight = 8;
        float[] chunkColor = new float[]{
            1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1
        };

        for(int y = 0; y < chunkHeight; y++){
            for(int z = 0; z < chunkDepth; z++){
                for(int x = 0; x < chunkWidth; x++){
                    SpawnCube(6, new Vector3f(x, y, z), chunkColor);
                }
            }
        }

    }

    @Override
    protected void OnUpdate() {

    }

    @Override
    protected void OnImGuiRender() {
        ImGui.begin("Debug");

        ImGui.separator();

        String fpsCounter = String.format("FPS: %.0f", Time.GetFPS());
        ImGui.text(fpsCounter);
        String deltaTimeCounter = String.format("Delta Time: %f", Time.GetDeltaTime());
        ImGui.text(deltaTimeCounter);

        ImGui.end();
    }
    
}
