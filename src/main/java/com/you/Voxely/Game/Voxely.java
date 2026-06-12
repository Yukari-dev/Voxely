package com.you.Voxely.Game;

import java.util.List;

import com.you.Voxely.Engine.VoxelyEngine;
import com.you.Voxely.Game.Blocks.BlockRegistry;
import com.you.Voxely.Game.ChunkSystem.Chunk;
import com.you.Voxely.Game.ChunkSystem.ChunkMeshBuilder;
import com.you.Voxely.Game.ChunkSystem.World;
import com.you.Voxely.Game.TerrainGeneration.NoiseLayer;
import com.you.Voxely.Game.TerrainGeneration.TerrainGenerator;
import com.you.Voxely.Game.Textures.TextureAtlasRegistery;
import com.you.Voxely.Mesh.Mesh;
import com.you.Voxely.Time.Time;

import imgui.ImGui;
import imgui.type.ImBoolean;

public class Voxely extends VoxelyEngine {
    private World world;
    
    private final int[] renderDistanceValue = new int[]{3}; 
    private ImBoolean syncEnability = new ImBoolean(false);
    private imgui.type.ImBoolean layerToggle = new imgui.type.ImBoolean(true);
    private boolean pendingRegeneration = false;

    @Override
    protected void OnStart() {
        TextureAtlasRegistery.LoadTextures();
        BlockRegistry.LoadDefinitions();
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
        if(pendingRegeneration){
            BuildActiveWorldGeometry();
            pendingRegeneration = false;
        }
    }

    @Override
    protected void OnImGuiRender() {
        ImGui.begin("Debug");

        String fpsCounter = String.format("FPS: %.0f", Time.GetFPS());
        ImGui.text(fpsCounter);
        ImGui.separator();

        ImGui.text("Modular Terrain Layers");
        
        TerrainGenerator generator = world.GetTerrainGenerator();
        List<NoiseLayer> layers = generator.GetLayers();

        for (int i = 0; i < layers.size(); i++) {
            NoiseLayer layer = layers.get(i);
            
            if (ImGui.treeNode(layer.name + "##" + i)) {
                
                layerToggle.set(layer.enabled);
                if (ImGui.checkbox("Enabled", layerToggle)) {
                    layer.enabled = layerToggle.get();
                    pendingRegeneration = true;
                }

                if (ImGui.sliderFloat("Frequency", layer.imguiFreq, 0.0001f, 0.05f)) pendingRegeneration = true;
                if (ImGui.sliderFloat("Amplitude", layer.imguiAmp, 0.0f, 64.0f))    pendingRegeneration = true;
                if (ImGui.sliderFloat("Base Height", layer.imguiBase, -20.0f, 50.0f)) pendingRegeneration = true;

                ImGui.treePop();
            }
            ImGui.spacing();
        }

        ImGui.separator();

        if (ImGui.button("Add New Custom Layer")) {
            layers.add(new NoiseLayer("Custom Layer " + (layers.size() + 1), 0.01f, 10.0f, 0.0f));
            pendingRegeneration = true;
        }

        ImGui.spacing();
        if (ImGui.sliderInt("Render Distance", renderDistanceValue, 1, 32)) {
            world.SetRenderDistance(renderDistanceValue[0]);
        }

        if (ImGui.button("Apply and Regenerate Chunk Grid")) {
            pendingRegeneration = true;
        }

        ImGui.separator();

        if(ImGui.checkbox("Sync", syncEnability)){
            GetWindow().SetSyncEnablility(syncEnability.get());
        }

        ImGui.end();
    }
}
