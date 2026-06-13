package com.you.Voxely.Game.GUI;

import com.you.Voxely.Game.Lightning.LightEnvironment;

import imgui.ImGui;
import imgui.type.ImInt;

public class LightningSettingsWindow implements GuiWindow{
    private final float[] sunDirection = {0.0f, 1.0f, 0.0f};
    private final float[] sunColor = {1.0f, 1.0f, 1.0f};
    private final float[] sunIntensity = {0.8f};
    
    private static float[] ambientColor = {1.0f, 1.0f, 1.0f};
    private static float[] ambientIntensity = {0.3f};

    private static ImInt selectedItem = new ImInt(0);
    private static String[] lightPresets = {"Noon", "SunSet"};


    @Override
    public String GetName() {
        return "Lightning Window";
    }

    @Override
    public void Render() {
        ImGui.begin("Light");

        if(ImGui.combo("Light Presets", selectedItem, lightPresets)){
            System.out.println("Selected: " + lightPresets[selectedItem.get()]);
        }

        if(ImGui.sliderFloat3("Sun Direction", sunDirection, -1.0f, 1.f)){
            LightEnvironment.sunDirection.x = sunDirection[0];
            LightEnvironment.sunDirection.y = sunDirection[1];
            LightEnvironment.sunDirection.z = sunDirection[2];
        }

        if(ImGui.colorEdit3("Sun Color", sunColor)){
            LightEnvironment.sunColor.x = sunColor[0];
            LightEnvironment.sunColor.y = sunColor[1];
            LightEnvironment.sunColor.z = sunColor[2];
        }

        if(ImGui.sliderFloat("Sun Intensity", sunIntensity, 0.0f, 100.0f)){
            LightEnvironment.sunIntensity = sunIntensity[0];
        }

        ImGui.separator();

        if(ImGui.colorEdit3("Ambient Color", ambientColor)){
            LightEnvironment.ambientColor.x = ambientColor[0];
            LightEnvironment.ambientColor.y = ambientColor[1];
            LightEnvironment.ambientColor.z = ambientColor[2];
        }

        if(ImGui.sliderFloat("Ambient Intensity", ambientIntensity, 0.0f, 100.0f)){
            LightEnvironment.ambientIntensity = ambientIntensity[0];
        }

        ImGui.end();
    }

    @Override
    public void Save() {
    }

    @Override
    public void Load() {
    }
    
}
