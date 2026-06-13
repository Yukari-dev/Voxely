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
    
    private final int[] renderDistanceValue = new int[]{3}; 
    private ImBoolean syncEnability = new ImBoolean(false);

    @Override
    protected void OnStart() {
        TextureAtlasRegistery.LoadTextures();
        BlockRegistry.LoadDefinitions();

        BuildActiveWorldGeometry();
    }

    private void BuildActiveWorldGeometry(){
        ClearChunkMeshes();
        
        GetWorld().ClearActiveChunks();
        GetWorld().GenerateTerrain(); 

        for(Chunk chunk : GetWorld().GetActiveChunks().values()){
            Mesh bakedMesh = ChunkMeshBuilder.GenerateChunkMesh(GetWorld(), chunk);
            CreateChunkMesh(bakedMesh);
        }
    }

    @Override
    protected void OnUpdate() {
        if(GetWorld().pendingRegeneration){
            BuildActiveWorldGeometry();
            GetWorld().pendingRegeneration = false;
        }
    }

    @Override
    protected void OnImGuiRender() {
        ImGui.begin("Debug");

        if (ImGui.sliderInt("Render Distance", renderDistanceValue, 1, 32)) {
            GetWorld().SetRenderDistance(renderDistanceValue[0]);
        }

        ImGui.end();
    }
}
