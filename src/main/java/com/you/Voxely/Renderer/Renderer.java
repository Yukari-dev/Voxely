package com.you.Voxely.Renderer;

import java.util.ArrayList;
import java.util.List;

import com.you.Voxely.Mesh.*;
import com.you.Voxely.Mesh.MeshCreator.MeshType;

public class Renderer {
    private Shader shader;
    private List<Mesh> meshes = new ArrayList<>();

    public Renderer(){
        shader = new Shader("DefaultVert.glsl", "DefaultFrag.glsl");
    }

    public void CreateMesh(MeshType meshType){
        Mesh mesh = MeshCreator.CreateMesh(meshType);
        AddMesh(mesh);
    }

    public void CreateMesh(Mesh mesh){
        AddMesh(mesh);
    }

    private void AddMesh(Mesh mesh){
        meshes.add(mesh);
    }

    public void Draw(){
        shader.Use();
        for (Mesh mesh : meshes) {
            mesh.Draw();
        }
        shader.UnUse();
    }
}
