package com.you.Voxely.Mesh;

import org.joml.Vector3f;

public class MeshCreator {
    public static enum MeshType{
        CUBE
    };

    public static enum FaceDirection{
        FRONT, BACK, RIGHT, LEFT, TOP, BOTTOM
    };

    public static Mesh CreateMesh(MeshType meshType, int faceCount, Vector3f position, float[] color){
        switch (meshType) {
            case CUBE:
                return CreateFace(faceCount, position, color);
            default:
                return null;
        }
    }

    private static Mesh CreateFace(int facesCount, Vector3f position, float[] faceColors){
        float[] vertices = new float[facesCount*4*3];
        int[] indices = new int[facesCount*3*2];
        float[] colors = new float[facesCount * 4 * 3];
        float unit = 0.5f;
        for(int face = 0; face < facesCount; face++){
            FaceDirection dir = FaceDirection.values()[face];
            int vOffset = face * 12;
            int iOffset = face * 6;

            int cIndex = face * 3;
            float r = faceColors[cIndex];
            float g = faceColors[cIndex + 1];
            float b = faceColors[cIndex + 2];

            colors[vOffset]     = r; colors[vOffset + 1] = g; colors[vOffset + 2] = b;
            colors[vOffset + 3] = r; colors[vOffset + 4] = g; colors[vOffset + 5] = b;
            colors[vOffset + 6] = r; colors[vOffset + 7] = g; colors[vOffset + 8] = b;
            colors[vOffset + 9] = r; colors[vOffset + 10]= g; colors[vOffset + 11]= b;

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
        return new Mesh(position, vertices, indices, colors);
    }

    public static Mesh CreateUnitCube(Vector3f position, int faces, float[] color){
        return CreateFace(faces, position, color);
    }
}
