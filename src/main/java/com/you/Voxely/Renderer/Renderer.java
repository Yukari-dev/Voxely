package com.you.Voxely.Renderer;

import java.util.ArrayList;
import java.util.List;

import org.joml.Vector3f;

import com.you.Voxely.Engine.Camera;
import com.you.Voxely.Game.Lightning.LightEnvironment;
import com.you.Voxely.Mesh.*;
import com.you.Voxely.Mesh.MeshCreator.MeshType;

public class Renderer {
    private Shader shader;
    private LightEnvironment lightEnvironment = LightEnvironment.createNoonPreset();
    private List<Mesh> meshes = new ArrayList<>();
    private List<Mesh> chunkMeshes = new ArrayList<>();
    private UniformBlock uniforms = new UniformBlock();
    private Camera camera;

    public Renderer(Camera camera){
        shader = new Shader("DefaultVert.glsl", "DefaultFrag.glsl");
        this.camera = camera;
    }

    public void CreateMesh(MeshType meshType, int faceCount, Vector3f position, float[] color){
        Mesh mesh = MeshCreator.CreateMesh(meshType, faceCount, position, color);
        AddMesh(mesh);
    }

    public void CreateMesh(Mesh mesh){
        AddMesh(mesh);
    }

    private void AddMesh(Mesh mesh){
        meshes.add(mesh);
    }

    public void CreateChunkMesh(Mesh mesh){
        AddChunkMesh(mesh);
    }

    private void AddChunkMesh(Mesh mesh){
        chunkMeshes.add(mesh);
    }

    public void ClearChunkMeshes(){
        for (Mesh mesh : chunkMeshes) {
            mesh.CleanUp();
        }
        chunkMeshes.clear();
    }

    public void Draw(){
        uniforms.Clear();
        uniforms.Set("projection", camera.GetProjectionMatrix());
        uniforms.Set("view", camera.GetViewMatrix());
        shader.ApplyUniforms(uniforms);
        shader.UploadLighting(lightEnvironment);

        for (Mesh mesh : meshes) {
            shader.SetMatrix("model", mesh.GetModelMatrix());
            mesh.Draw(shader);
        }

        for (Mesh mesh : chunkMeshes) {
            shader.SetMatrix("model", mesh.GetModelMatrix());
            mesh.Draw(shader);
        }
        shader.UnUse();
    }
}
