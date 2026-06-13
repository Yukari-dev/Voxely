package com.you.Voxely.Game.GUI;

import imgui.ImGui;
import imgui.type.ImBoolean;
import com.you.Voxely.Game.ChunkSystem.World;
import com.you.Voxely.Game.TerrainGeneration.NoiseLayer;

public class TerrainSettingsWindow implements GuiWindow {
    private final World world;
    private final int[] renderDistanceValue = new int[]{3}; 

    private final String[] noiseLabels = { "Simplex Smooth", "OpenSimplex2", "Cellular (Voronoi)", "Classic Perlin" };
    private final String[] fractalLabels = { "None (Raw)", "FBm (Standard)", "Ridged Sharp", "PingPong (Caves/Dunes)" };

    public TerrainSettingsWindow(World world) {
        this.world = world;
    }

    @Override
    public String GetName() { return "Terrain Settings"; }

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

        ImGui.end();
    }
}