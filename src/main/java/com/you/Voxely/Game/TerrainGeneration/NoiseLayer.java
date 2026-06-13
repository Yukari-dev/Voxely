package com.you.Voxely.Game.TerrainGeneration;

import imgui.type.ImInt;

public class NoiseLayer {
    public String name;
    public boolean enabled = true;
    public int[] imguiSeed = new int[]{0};

    public float[] imguiFreq = new float[]{ 0.005f };
    public float[] imguiAmp = new float[]{ 32.0f };
    public float[] imguiOffs = new float[]{ 0.0f };

    public ImInt imguiNoiseType = new ImInt(1);
    public ImInt imguiFractalType = new ImInt(1);
    
    public int[] imguiOctaves = new int[]{ 3 };
    public float[] imguiLac = new float[]{ 2.0f };
    public float[] imguiGain = new float[]{ 0.5f };
    
    public NoiseLayer(String name, float frequency, float amplitude, float offset){
        this.name = name;
        this.imguiFreq[0] = frequency;
        this.imguiAmp[0] = amplitude;
        this.imguiOffs[0] = offset;
    }
}