package com.you.Voxely.Renderer;

import org.joml.Matrix4f;
import org.joml.Vector2f;
import org.joml.Vector3f;
import org.joml.Vector4f;
import java.util.LinkedHashMap;
import java.util.Map;

public class UniformBlock {

    private final Map<String, Object> uniforms = new LinkedHashMap<>();

    public UniformBlock Set(String name, Matrix4f value) {
        uniforms.put(name, new Matrix4f(value));
        return this;
    }
    public UniformBlock Set(String name, float value) {
        uniforms.put(name, value);
        return this;
    }
    public UniformBlock Set(String name, int value) {
        uniforms.put(name, value);
        return this;
    }
    public UniformBlock Set(String name, boolean value) {
        uniforms.put(name, value);
        return this;
    }
    public UniformBlock Set(String name, Vector2f value) {
        uniforms.put(name, new Vector2f(value));
        return this;
    }
    public UniformBlock Set(String name, Vector3f value) {
        uniforms.put(name, new Vector3f(value));
        return this;
    }
    public UniformBlock Set(String name, Vector4f value) {
        uniforms.put(name, new Vector4f(value));
        return this;
    }

    public Map<String, Object> GetAll() { return uniforms; }

    public void Clear() { uniforms.clear(); }
}