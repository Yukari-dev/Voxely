package com.you.Voxely.Game.TerrainGeneration;

import org.joml.Math;

public class NoiseLayer {
    public String name;
    public boolean enabled = true;
    public float frequency = 0.005f;
    public float amplitude = 32.0f;
    public float baseHeight = 10.0f;

    public float[] imguiFreq = new float[]{ 0.005f };
    public float[] imguiAmp = new float[]{ 32.0f };
    public float[] imguiBase = new float[]{ 10.0f };
    
    public NoiseLayer(String name, float frequency, float amplitude, float baseHeight){
        this.name = name;
        this.frequency = frequency;
        this.amplitude = amplitude;
        this.baseHeight = baseHeight;
        
        this.imguiFreq[0] = frequency;
        this.imguiAmp[0] = amplitude;
        this.imguiBase[0] = baseHeight;
    }

    public float GetNoise(float x, float z){
        if(!enabled) return 0;

        frequency = imguiFreq[0];
        amplitude = imguiAmp[0];
        baseHeight = imguiBase[0];

        double noiseValue = Math.sin(x*frequency)*Math.cos(z*frequency);
        return baseHeight + (float)(noiseValue*amplitude);
    }
}
