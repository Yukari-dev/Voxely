package com.you.Voxely.Game.ChunkSystem;

import java.util.HashMap;
import java.util.Map;

import org.joml.Vector3f;

import com.you.Voxely.Game.TerrainGeneration.TerrainGenerator;

public class World {
    private Map<ChunkPosition, Chunk> activeChunks = new HashMap<>();
    private TerrainGenerator terrainGenerator = new TerrainGenerator();
    private int renderDistance = 1;
    public boolean pendingRegeneration = false;

    public void GenerateTerrain(){
        for(int x = -renderDistance; x <= renderDistance; x++){
            for(int z = -renderDistance; z <= renderDistance; z++){
                ChunkPosition pos = new ChunkPosition(x, 0, z);
                Vector3f worldRealPos = new Vector3f(x*Chunk.SIZE, 0, z*Chunk.SIZE);
                activeChunks.put(pos, new Chunk(worldRealPos));
            }
        }
        for(Chunk chunk : activeChunks.values()){
            terrainGenerator.GenerateChunkTerrain(chunk);
        }
    }

    public void RegenerateWorld() {
        ClearActiveChunks();
        
        for(int x = -renderDistance; x <= renderDistance; x++){
            for(int z = -renderDistance; z <= renderDistance; z++){
                ChunkPosition pos = new ChunkPosition(x, 0, z);
                Vector3f worldRealPos = new Vector3f(x * Chunk.SIZE, 0, z * Chunk.SIZE);
                activeChunks.put(pos, new Chunk(worldRealPos));
            }
        }
        
        for(Chunk chunk : activeChunks.values()){
            terrainGenerator.GenerateChunkTerrain(chunk);
        }
    }

    public TerrainGenerator GetTerrainGenerator(){
        return terrainGenerator;
    }

    public void ClearActiveChunks() {
        activeChunks.clear();
    }

    public boolean IsBlockSolidAt(int globalX, int globalY, int globalZ) {
        int chunkX = (int) Math.floor((double) globalX / Chunk.SIZE);
        int chunkY = (int) Math.floor((double) globalY / Chunk.HEIGHT);
        int chunkZ = (int) Math.floor((double) globalZ / Chunk.SIZE);

        ChunkPosition targetKey = new ChunkPosition(chunkX, chunkY, chunkZ);
        Chunk targetChunk = activeChunks.get(targetKey);

        if (targetChunk == null) return false;

        int localX = ((globalX % Chunk.SIZE) + Chunk.SIZE) % Chunk.SIZE;
        int localY = ((globalY % Chunk.HEIGHT) + Chunk.HEIGHT) % Chunk.HEIGHT;
        int localZ = ((globalZ % Chunk.SIZE) + Chunk.SIZE) % Chunk.SIZE;

        return targetChunk.IsSolid(localX, localY, localZ);
    }

    public void SetRenderDistance(int renderDistance){
        this.renderDistance = renderDistance;
    }

    public int GetRenderDistance(){
        return renderDistance;
    }

    public Map<ChunkPosition, Chunk> GetActiveChunks(){
        return activeChunks;
    }
}
