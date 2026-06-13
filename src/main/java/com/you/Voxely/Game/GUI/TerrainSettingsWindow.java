package com.you.Voxely.Game.GUI;

import imgui.ImGui;
import imgui.type.ImBoolean;
import com.you.Voxely.Game.ChunkSystem.World;
import com.you.Voxely.Game.TerrainGeneration.NoiseLayer;

public class TerrainSettingsWindow implements GuiWindow {
    private final World world;
    private final int[] renderDistanceValue = new int[]{3}; 

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

                if (ImGui.sliderFloat("Frequency##" + layer.name, layer.imguiFreq, 0.0001f, 0.05f)) {
                    world.pendingRegeneration = true;
                }

                if (ImGui.sliderFloat("Amplitude##" + layer.name, layer.imguiAmp, 0.0f, 128.0f)) {
                    world.pendingRegeneration = true;
                }

                if(ImGui.sliderFloat("BaseHeight##" + layer.name, layer.imguiBase, -20.0f, 50.0f)){
                    world.pendingRegeneration = true;
                }
            }
        }

        ImGui.separator();

        if(ImGui.sliderInt("Render Distance", renderDistanceValue, 1, 128)){
            world.SetRenderDistance(renderDistanceValue[0]);
        }

        if (ImGui.button("Regenerate World Chunks")) {
            world.pendingRegeneration = true;
        }

        ImGui.end();
    }
}