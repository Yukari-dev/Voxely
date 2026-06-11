package com.you.Voxely.Game;

import com.you.Voxely.Engine.VoxelyEngine;
import com.you.Voxely.Game.ChunkSystem.Chunk;
import com.you.Voxely.Game.ChunkSystem.ChunkMeshBuilder;
import com.you.Voxely.Game.ChunkSystem.World;
import com.you.Voxely.Mesh.Mesh;
import com.you.Voxely.Time.Time;

import imgui.ImGui;
import imgui.type.ImBoolean;

public class Voxely extends VoxelyEngine {
    private World world;
    
    private final int[] renderDistanceValue = new int[]{3}; 
    private ImBoolean syncEnability = new ImBoolean(false);

    @Override
    protected void OnStart() {
        world = new World();
        world.SetRenderDistance(4);

        BuildActiveWorldGeometry();
    }

    private void BuildActiveWorldGeometry(){
        ClearChunkMeshes();
        world.ClearActiveChunks();
        world.GenerateTerrain();

        for(Chunk chunk : world.GetActiveChunks().values()){
            Mesh bakedMesh = ChunkMeshBuilder.GenerateChunkMesh(world, chunk);
            CreateChunkMesh(bakedMesh);
        }
    }

    @Override
    protected void OnUpdate() {

    }

    @Override
    protected void OnImGuiRender() {
        ImGui.begin("Debug");

        String fpsCounter = String.format("FPS: %.0f", Time.GetFPS());
        ImGui.text(fpsCounter);
        String deltaTimeCounter = String.format("Delta Time: %fms", Time.GetDeltaTime());
        ImGui.text(deltaTimeCounter);

        ImGui.separator();
        Runtime runtime = Runtime.getRuntime();
        long usedMemoryBytes = runtime.totalMemory() - runtime.freeMemory();
        double usedMemoryMegabytes = usedMemoryBytes / (1024.0 * 1024.0);
        double maxMemoryMegabytes = runtime.maxMemory() / (1024.0 * 1024.0);

        ImGui.text(String.format("Used Heap: %.2f MB", usedMemoryMegabytes));
        ImGui.text(String.format("Max Heap:  %.2f MB", maxMemoryMegabytes));

        
        ImGui.separator();

        if (ImGui.sliderInt("Render Distance", renderDistanceValue, 1, 32)) {
            world.SetRenderDistance(renderDistanceValue[0]);
        }

        if (ImGui.button("Apply and Regenerate Chunk Grid")) {
            BuildActiveWorldGeometry();
        }

        ImGui.separator();
        
        if(ImGui.checkbox("Sync", syncEnability)){
            GetWindow().SetSyncEnablility(syncEnability.get());
        }

        ImGui.end();
    }
    
}
