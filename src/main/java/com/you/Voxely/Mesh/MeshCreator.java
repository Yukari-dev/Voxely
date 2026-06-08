package com.you.Voxely.Mesh;

import org.joml.Vector3f;

public class MeshCreator {
    public static enum MeshType{
        CUBE
    };

    public static Mesh CreateMesh(MeshType meshType, Vector3f position){
        switch (meshType) {
            case CUBE:
                return CreateUnitCube(position);
            default:
                return null;
        }
    }

    private static Mesh CreateUnitCube(Vector3f position){
        float[] vertices = {
            0.5f,  0.5f, 0.0f, 
            0.5f, -0.5f, 0.0f, 
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f  
        };
        int[] indices = {  
            0, 1, 3,
            1, 2, 3 
        };  
        VertexLayout layout = new VertexLayout().Add(VertexLayout.AttributeType.POSITION);
        return new Mesh(new Vector3f(position), vertices, indices, layout);
    }
}
