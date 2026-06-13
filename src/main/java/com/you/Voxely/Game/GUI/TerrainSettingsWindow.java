package com.you.Voxely.Game.GUI;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.you.Voxely.Game.ChunkSystem.World;
import com.you.Voxely.Game.TerrainGeneration.NoiseLayer;

import imgui.ImGui;
import imgui.type.ImBoolean;

public class TerrainSettingsWindow implements GuiWindow {
    private final World world;
    private final int[] renderDistanceValue = new int[]{3}; 

    private static final String SAVE_FILE = "src/main/resources/Data/Saves/config_terrain.json";
    private final Gson gson = new GsonBuilder().setPrettyPrinting().create();

    private final String[] noiseLabels = { "Simplex Smooth", "OpenSimplex2", "Cellular (Voronoi)", "Classic Perlin" };
    private final String[] fractalLabels = { "None (Raw)", "FBm (Standard)", "Ridged Sharp", "PingPong (Caves/Dunes)" };

    public static class LayerSaveData{
        String name;
        boolean enabled;
        int seed;
        float frequency;
        float amplitude;
        float offset;
        int noiseType;
        int fractalType;
        int octaves;
        float lacunarity;
        float gain;
    }

    public static class TerrainConfig{
        int renderDistance;
        List<LayerSaveData> layers = new ArrayList<>();
    }

    public TerrainSettingsWindow(World world) {
        this.world = world;
        Load();
    }

    @Override
    public String GetName() { return "Terrain Settings"; }

    @Override
    public void Save(){
        try(FileWriter writer = new FileWriter(SAVE_FILE)){
            TerrainConfig config = new TerrainConfig();
            config.renderDistance = renderDistanceValue[0];
            for(NoiseLayer layer : world.GetTerrainGenerator().GetLayers()){
                LayerSaveData data = new LayerSaveData();
                data.name = layer.name;
                data.enabled = layer.enabled;
                data.seed = layer.imguiSeed[0];
                data.frequency = layer.imguiFreq[0];
                data.amplitude = layer.imguiAmp[0];
                data.offset = layer.imguiOffs[0];
                data.noiseType = layer.imguiNoiseType.get();
                data.fractalType = layer.imguiFractalType.get();
                data.octaves = layer.imguiOctaves[0];
                data.lacunarity = layer.imguiLac[0];
                data.gain = layer.imguiGain[0];
                config.layers.add(data);
            }
            gson.toJson(config, writer);
        } catch(IOException e){
            System.err.println("Failed to write terrain config save: " + e.getMessage());
        }
    }

    @Override
    public void Load(){
        File file = new File(SAVE_FILE);
        if(!file.exists()) return;

        try(FileReader reader = new FileReader(SAVE_FILE)){
            TerrainConfig config = gson.fromJson(reader, TerrainConfig.class);
            if(config == null) return;

            renderDistanceValue[0] = config.renderDistance;
            world.SetRenderDistance(config.renderDistance);

            for(LayerSaveData savedLayer : config.layers){
                for(NoiseLayer currentLayer : world.GetTerrainGenerator().GetLayers()){
                    if(currentLayer.name.equals(savedLayer.name)){
                        currentLayer.enabled = savedLayer.enabled;
                        currentLayer.imguiSeed[0] = savedLayer.seed;
                        currentLayer.imguiFreq[0] = savedLayer.frequency;
                        currentLayer.imguiAmp[0] = savedLayer.amplitude;
                        currentLayer.imguiOffs[0] = savedLayer.offset;
                        currentLayer.imguiNoiseType.set(savedLayer.noiseType);
                        currentLayer.imguiFractalType.set(savedLayer.fractalType);
                        currentLayer.imguiOctaves[0] = savedLayer.octaves;
                        currentLayer.imguiLac[0] = savedLayer.lacunarity;
                        currentLayer.imguiGain[0] = savedLayer.gain;
                    }
                }
            }
            world.pendingRegeneration = true;
        } catch(IOException e){
            System.err.println("Failed to parse terrain configuration profile: " + e.getMessage());
        }
    }

    @Override
    public void Render() {
        ImGui.begin("Terrain Generation Layers");

        for (NoiseLayer layer : world.GetTerrainGenerator().GetLayers()) {
            if (ImGui.collapsingHeader(layer.name)) {
                
                ImBoolean enabled = new ImBoolean(layer.enabled);
                if (ImGui.checkbox("Enabled##" + layer.name, enabled)) {
                    layer.enabled = enabled.get();
                    world.pendingRegeneration = true;
                }

                if(ImGui.sliderInt("World Seed##"+layer.name, layer.imguiSeed, -10000, 10000)){
                    world.pendingRegeneration = true;
                }

                if (ImGui.combo("Noise Algorithm##" + layer.name, layer.imguiNoiseType, noiseLabels)) {
                    world.pendingRegeneration = true;
                }

                if (ImGui.combo("Fractal Modifier##" + layer.name, layer.imguiFractalType, fractalLabels)) {
                    world.pendingRegeneration = true;
                }

                ImGui.separator();

                if (ImGui.sliderFloat("Frequency##" + layer.name, layer.imguiFreq, 0.0001f, 0.05f)) {
                    world.pendingRegeneration = true;
                }

                if (ImGui.sliderFloat("Amplitude##" + layer.name, layer.imguiAmp, 0.0f, 128.0f)) {
                    world.pendingRegeneration = true;
                }

                if (ImGui.sliderFloat("Height Offset Y##" + layer.name, layer.imguiOffs, -50.0f, 120.0f)) {
                    world.pendingRegeneration = true;
                }

                if (layer.imguiFractalType.get() > 0) {
                    if (ImGui.sliderInt("Octaves (Detail Layers)##" + layer.name, layer.imguiOctaves, 1, 8)) {
                        world.pendingRegeneration = true;
                    }

                    if (ImGui.sliderFloat("Lacunarity (Gaps)##" + layer.name, layer.imguiLac, 1.0f, 4.0f)) {
                        world.pendingRegeneration = true;
                    }

                    if (ImGui.sliderFloat("Gain (Roughness)##" + layer.name, layer.imguiGain, 0.0f, 1.0f)) {
                        world.pendingRegeneration = true;
                    }
                }
            }
        }

        ImGui.spacing();
        ImGui.separator();
        ImGui.spacing();

        if (ImGui.sliderInt("Render Distance", renderDistanceValue, 1, 128)) {
            world.SetRenderDistance(renderDistanceValue[0]);
        }

        if (ImGui.button("Regenerate World Chunks")) {
            world.pendingRegeneration = true;
        }


        if (ImGui.button("SAVE")) {
            Save();
        }

        ImGui.end();
    }
}