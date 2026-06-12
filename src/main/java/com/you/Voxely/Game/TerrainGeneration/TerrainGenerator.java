package com.you.Voxely.Game.TerrainGeneration;

import java.util.ArrayList;
import java.util.List;

import com.you.Voxely.Game.ChunkSystem.Chunk;

public class TerrainGenerator {
    
    private List<NoiseLayer> layers;

    public TerrainGenerator(){
        layers = new ArrayList<>();
        layers.add(new NoiseLayer("Base Landscape", 0.005f, 16.0f, 12.0f));
        layers.add(new NoiseLayer("Rough Mountains", 0.03f, 8.0f, 0.0f));
        layers.add(new NoiseLayer("Micro Detail Humps", 0.1f, 2.0f, 0.0f));
    }

    public void AddLayer(NoiseLayer layer){
        layers.add(layer);
    }
    
    public List<NoiseLayer> GetLayers(){
        return layers;
    }

    public int CalculateSurfaceHeight(float globalX, float globalZ){
        float finalHeight = 0;
        for(NoiseLayer layer : layers){
            if(layer.enabled){
                finalHeight += layer.GetNoise(globalX, globalZ);
            }
        }
        return Math.max(1, (int)finalHeight);
    }
    
    public void GenerateChunkTerrain(Chunk chunk){
        int chunkX = (int)chunk.GetPosition().x;
        int chunkY = (int)chunk.GetPosition().y;
        int chunkZ = (int)chunk.GetPosition().z;

        for(int x = 0; x < Chunk.SIZE; x++){
            for(int z = 0; z < Chunk.SIZE; z++){
                int globalX = chunkX+x;
                int globalZ = chunkZ+z;

                int surfaceHeight = CalculateSurfaceHeight(globalX, globalZ);

                for(int y = 0; y < Chunk.HEIGHT; y++){
                    int globalY = chunkY+y;
                    if(globalY <= surfaceHeight){
                        chunk.SetBlockType(x, y, z, 1);
                    } else{
                        chunk.SetBlockType(x, y, z, 0);
                    }
                }
            }
        }
    }
}
