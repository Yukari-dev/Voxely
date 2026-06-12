package com.you.Voxely.Game.Lightning;

import org.joml.Vector3f;

public class LightEnvironment {
    public Vector3f sunDirection;
    public Vector3f sunColor;
    public float sunIntensity;
    
    public Vector3f ambientColor;
    public float ambientIntensity;

    
    public static LightEnvironment createNoonPreset() {
        LightEnvironment le = new LightEnvironment();
        le.sunDirection = new Vector3f(0.3f, 1.0f, 0.2f);
        le.sunColor = new Vector3f(1.0f, 0.98f, 0.9f);
        le.sunIntensity = 0.8f;
        le.ambientColor = new Vector3f(1.0f, 1.0f, 1.0f);
        le.ambientIntensity = 0.3f;
        return le;
    }

    public static LightEnvironment createSunsetPreset() {
        LightEnvironment le = new LightEnvironment();
        le.sunDirection = new Vector3f(0.8f, 0.2f, 0.1f);
        le.sunColor = new Vector3f(1.0f, 0.4f, 0.1f);
        le.sunIntensity = 0.6f;
        le.ambientColor = new Vector3f(0.4f, 0.4f, 0.6f);
        le.ambientIntensity = 0.25f;
        return le;
    }
}
