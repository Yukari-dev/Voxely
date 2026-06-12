package com.you.Voxely.Renderer;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import static org.lwjgl.opengl.GL46.*;
import org.lwjgl.system.MemoryStack;

import com.you.Voxely.Game.Lightning.LightEnvironment;

import org.joml.Matrix4f;
import org.joml.Vector2f;
import org.joml.Vector3f;
import org.joml.Vector4f;

public class Shader{
    private int programID;

    public Shader(String vertexPath, String fragPath){
        String shadersPath = "src/main/resources/shaders/";
        String vertexSource = GetShaderSource(shadersPath+vertexPath);
        String fragSource = GetShaderSource(shadersPath+fragPath);
        int vertexId = CompileShader(vertexSource, GL_VERTEX_SHADER);
        int fragId = CompileShader(fragSource, GL_FRAGMENT_SHADER);
        programID = CreateProgram(vertexId, fragId);
    }

    private int CompileShader(String source, int type){
        int shader = glCreateShader(type);
        glShaderSource(shader, source);
        glCompileShader(shader);
        int success = glGetShaderi(shader, GL_COMPILE_STATUS);
        if (success == GL_FALSE) {
            throw new RuntimeException("Shader compilation failed: " + glGetShaderInfoLog(shader, 512));
        }
        return shader;
    }

    private int CreateProgram(int vertexShader, int fragShader){
        int id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragShader);
        glLinkProgram(id);
        int success = glGetProgrami(id, GL_LINK_STATUS);
        if (success == GL_FALSE) {
            throw new RuntimeException("Program linking failed: " + glGetProgramInfoLog(id, 512));
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader); 
        return id;
    }

    public void Use(){
        glUseProgram(programID);
    }

    public void UnUse(){
        glUseProgram(0);
    }

    public int GetProgramID(){
        return programID;
    }

    public void SetMatrix(String name, Matrix4f matrix){
        int loc = GetLocation(name);
        try (MemoryStack stack = MemoryStack.stackPush()) {
            glUniformMatrix4fv(
                    loc,
                    false,
                    matrix.get(stack.mallocFloat(16))
            );
        }
    }

    public void SetBool(String name, boolean value){
        int loc = GetLocation(name);
        glUniform1i(loc, value ? 1 : 0);
    }

    public void SetInt(String name, int value){
        int loc = GetLocation(name);
        glUniform1i(loc, value);
    }

    public void SetFloat(String name, float value){
        int loc = GetLocation(name);
        glUniform1f(loc, value);
    }

    public void SetVec2(String name, Vector2f value){
        int loc = GetLocation(name);
        glUniform2f(loc, value.x, value.y);
    }

    public void SetVec3(String name, Vector3f value){
        int loc = GetLocation(name);
        glUniform3f(loc, value.x, value.y, value.z);
    }

    public void SetVec4(String name, Vector4f value) {
        glUniform4f(GetLocation(name), value.x, value.y, value.z, value.w);
    }

    public void ApplyUniforms(UniformBlock block) {
        Use();
        for (var entry : block.GetAll().entrySet()) {
            String name  = entry.getKey();
            Object value = entry.getValue();

            if      (value instanceof Matrix4f m)  SetMatrix(name, m);
            else if (value instanceof Float f)      SetFloat(name, f);
            else if (value instanceof Integer i)    SetInt(name, i);
            else if (value instanceof Boolean b)    SetBool(name, b);
            else if (value instanceof Vector2f v)   SetVec2(name, v);
            else if (value instanceof Vector3f v)   SetVec3(name, v);
            else if (value instanceof Vector4f v)   SetVec4(name, v);
        }
    }

    public void UploadLighting(LightEnvironment env) {
        SetVec3("sunDirection", env.sunDirection);
        SetVec3("sunColor", env.sunColor);
        SetFloat("sunIntensity", env.sunIntensity);
        
        SetVec3("ambientColor", env.ambientColor);
        SetFloat("ambientIntensity", env.ambientIntensity);
    }

    public int GetLocation(String name){
        return glGetUniformLocation(programID, name);
    }

    String GetShaderSource(String path){
        try{
            byte[] bytes = Files.readAllBytes(Paths.get(path));
            String src = new String(bytes, StandardCharsets.UTF_8);
            if (src.startsWith("\uFEFF")) src = src.substring(1);
            return src.trim();
        } catch (IOException e) {
            throw new RuntimeException("Could not read shader file: " + path, e);
        }
    }
}

