package com.you.Voxely.Game.Lightning;

import org.joml.Vector3f;

public class LightEnvironment {
    public static Vector3f sunDirection;
    public static Vector3f sunColor;
    public static float sunIntensity;
    
    public static Vector3f ambientColor;
    public static float ambientIntensity;

    public static void CreateNoonPreset() {
        sunDirection = new Vector3f(0.3f, 1.0f, 0.2f);
        sunColor = new Vector3f(1.0f, 0.98f, 0.9f);
        sunIntensity = 0.8f;
        ambientColor = new Vector3f(1.0f, 1.0f, 1.0f);
        ambientIntensity = 0.3f;
    }

    public static void CreateSunsetPreset() {
        sunDirection = new Vector3f(0.8f, 0.2f, 0.1f);
        sunColor = new Vector3f(1.0f, 0.4f, 0.1f);
        sunIntensity = 0.6f;
        ambientColor = new Vector3f(0.4f, 0.4f, 0.6f);
        ambientIntensity = 0.25f;
    }
}
