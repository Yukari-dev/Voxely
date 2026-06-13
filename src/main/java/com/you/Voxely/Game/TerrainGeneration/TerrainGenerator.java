package com.you.Voxely.Game.TerrainGeneration;

import java.util.ArrayList;
import java.util.List;
import com.you.Voxely.Game.ChunkSystem.Chunk;
import com.you.Voxely.Game.Blocks.BlockRegistry;

public class TerrainGenerator {
    private final List<NoiseLayer> layers;
    private final FastNoiseLite noiseInstance;

    private static final FastNoiseLite.NoiseType[] NOISE_TYPES = {
        FastNoiseLite.NoiseType.OpenSimplex2S,
        FastNoiseLite.NoiseType.OpenSimplex2,
        FastNoiseLite.NoiseType.Cellular,
        FastNoiseLite.NoiseType.Perlin
    };

    private static final FastNoiseLite.FractalType[] FRACTAL_TYPES = {
        FastNoiseLite.FractalType.None,
        FastNoiseLite.FractalType.FBm,
        FastNoiseLite.FractalType.Ridged,
        FastNoiseLite.FractalType.PingPong
    };

    public TerrainGenerator() {
        this.layers = new ArrayList<>();
        this.noiseInstance = new FastNoiseLite();

        layers.add(new NoiseLayer("Base Landscape", 0.005f, 35.0f, 60.0f));
        layers.add(new NoiseLayer("Rough Mountains", 0.02f, 15.0f, 0.0f));
        layers.add(new NoiseLayer("Micro Detail Humps", 0.08f, 3.0f, 0.0f));
    }

    public int CalculateSurfaceHeight(float globalX, float globalZ) {
        float finalHeight = 0;

        for (NoiseLayer layer : layers) {
            if (!layer.enabled) continue;

            noiseInstance.SetNoiseType(NOISE_TYPES[layer.imguiNoiseType.get()]);
            noiseInstance.SetFractalType(FRACTAL_TYPES[layer.imguiFractalType.get()]);
            noiseInstance.SetFrequency(layer.imguiFreq[0]);
            noiseInstance.SetFractalOctaves(layer.imguiOctaves[0]);
            noiseInstance.SetFractalLacunarity(layer.imguiLac[0]);
            noiseInstance.SetFractalGain(layer.imguiGain[0]);

            float noiseSample = noiseInstance.GetNoise(globalX, globalZ);
            
            finalHeight += (noiseSample * layer.imguiAmp[0]) + layer.imguiOffs[0];
        }

        return Math.max(1, (int)finalHeight);
    }

    public void GenerateChunkTerrain(Chunk chunk) {
        int chunkX = (int)chunk.GetPosition().x;
        int chunkZ = (int)chunk.GetPosition().z;

        short grassId = (short)BlockRegistry.GetBlock(1).id;
        short dirtId = (short)BlockRegistry.GetBlock(2).id;

        for (int x = 0; x < Chunk.SIZE; x++) {
            for (int z = 0; z < Chunk.SIZE; z++) {
                int globalX = chunkX + x;
                int globalZ = chunkZ + z;

                int surfaceHeight = CalculateSurfaceHeight(globalX, globalZ);

                for (int y = 0; y < Chunk.HEIGHT; y++) {
                    if (y > surfaceHeight) {
                        chunk.SetBlockType(x, y, z, (short)0);
                    } else if (y == surfaceHeight) {
                        chunk.SetBlockType(x, y, z, grassId);
                    } else if (y >= surfaceHeight - 3) {
                        chunk.SetBlockType(x, y, z, dirtId);
                    } else {
                        chunk.SetBlockType(x, y, z, dirtId);
                    }
                }
            }
        }
    }

    public List<NoiseLayer> GetLayers() { return layers; }
}