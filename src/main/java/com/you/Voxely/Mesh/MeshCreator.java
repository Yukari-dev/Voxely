package com.you.Voxely.Mesh;

import org.joml.Vector3f;

public class MeshCreator {
    public static enum MeshType{
        CUBE
    };

    public static enum FaceDirection{
        FRONT, BACK, RIGHT, LEFT, TOP, BOTTOM
    };

    public static Mesh CreateMesh(MeshType meshType, Vector3f position){
        switch (meshType) {
            case CUBE:
                return CreateFace(6, position);
            default:
                return null;
        }
    }

    private static Mesh CreateFace(int facesCount, Vector3f position){
        float[] vertices = new float[facesCount*4*3];
        int[] indices = new int[facesCount*3*2];
        float unit = 0.5f;
        for(int face = 0; face < facesCount; face++){
            FaceDirection dir = FaceDirection.values()[face];
            int vOffset = face * 12;
            int iOffset = face * 6;

            switch(dir){
                case FRONT:
                    vertices[vOffset]   = unit;
                    vertices[vOffset+1] = unit;
                    vertices[vOffset+2] = unit;

                    vertices[vOffset+3] = unit;
                    vertices[vOffset+4] = -unit;
                    vertices[vOffset+5] = unit;

                    vertices[vOffset+6] = -unit;
                    vertices[vOffset+7] = -unit;
                    vertices[vOffset+8] = unit;

                    vertices[vOffset+9]  = -unit;
                    vertices[vOffset+10] = unit;
                    vertices[vOffset+11] = unit;
                    break;
                case BACK:
                    vertices[vOffset]   = unit;
                    vertices[vOffset+1] = unit;
                    vertices[vOffset+2] = -unit;

                    vertices[vOffset+3] = unit;
                    vertices[vOffset+4] = -unit;
                    vertices[vOffset+5] = -unit;

                    vertices[vOffset+6] = -unit;
                    vertices[vOffset+7] = -unit;
                    vertices[vOffset+8] = -unit;

                    vertices[vOffset+9]  = -unit;
                    vertices[vOffset+10] = unit;
                    vertices[vOffset+11] = -unit;
                    break;
                case RIGHT:
                    vertices[vOffset]   = -unit;
                    vertices[vOffset+1] = unit;
                    vertices[vOffset+2] = unit;

                    vertices[vOffset+3] = -unit;
                    vertices[vOffset+4] = -unit;
                    vertices[vOffset+5] = unit;

                    vertices[vOffset+6] = -unit;
                    vertices[vOffset+7] = -unit;
                    vertices[vOffset+8] = -unit;

                    vertices[vOffset+9]  = -unit;
                    vertices[vOffset+10] = unit;
                    vertices[vOffset+11] = -unit;
                    break;
                case LEFT:
                    vertices[vOffset]   = unit;
                    vertices[vOffset+1] = unit;
                    vertices[vOffset+2] = unit;

                    vertices[vOffset+3] = unit;
                    vertices[vOffset+4] = -unit;
                    vertices[vOffset+5] = unit;

                    vertices[vOffset+6] = unit;
                    vertices[vOffset+7] = -unit;
                    vertices[vOffset+8] = -unit;

                    vertices[vOffset+9]  = unit;
                    vertices[vOffset+10] = unit;
                    vertices[vOffset+11] = -unit;
                    break;
                case TOP:
                    vertices[vOffset]   = unit;
                    vertices[vOffset+1] = unit;
                    vertices[vOffset+2] = unit;

                    vertices[vOffset+3] = -unit;
                    vertices[vOffset+4] = unit;
                    vertices[vOffset+5] = unit;

                    vertices[vOffset+6] = -unit;
                    vertices[vOffset+7] = unit;
                    vertices[vOffset+8] = -unit;

                    vertices[vOffset+9]  = unit;
                    vertices[vOffset+10] = unit;
                    vertices[vOffset+11] = -unit;
                    break;
                case BOTTOM:
                    vertices[vOffset]   = unit;
                    vertices[vOffset+1] = -unit;
                    vertices[vOffset+2] = unit;

                    vertices[vOffset+3] = -unit;
                    vertices[vOffset+4] = -unit;
                    vertices[vOffset+5] = unit;

                    vertices[vOffset+6] = -unit;
                    vertices[vOffset+7] = -unit;
                    vertices[vOffset+8] = -unit;

                    vertices[vOffset+9]  = unit;
                    vertices[vOffset+10] = -unit;
                    vertices[vOffset+11] = -unit;
                    break;
                default:
                    break;
            }

            int vertexStartIndex = face * 4;
            indices[iOffset] = vertexStartIndex;
            indices[iOffset+1] = vertexStartIndex+1;
            indices[iOffset+2] = vertexStartIndex+2;

            indices[iOffset+3] = vertexStartIndex;
            indices[iOffset+4] = vertexStartIndex+2;
            indices[iOffset+5] = vertexStartIndex+3;
        }
        VertexLayout layout = new VertexLayout().Add(VertexLayout.AttributeType.POSITION);
        return new Mesh(position, vertices, indices, layout);
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
