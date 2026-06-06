package com.you.Voxely.Engine.Mesh;

public class MeshCreator {
    public static enum MeshType{
        CUBE
    };

    public static Mesh CreateMesh(MeshType meshType){
        switch (meshType) {
            case CUBE:
                return CreateUnitCube();
            default:
                return null;
        }
    }

    private static Mesh CreateUnitCube(){
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
        return new Mesh(vertices, indices, layout);
    }
}
