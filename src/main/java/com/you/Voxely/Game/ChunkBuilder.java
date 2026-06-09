package com.you.Voxely.Game;

import java.util.ArrayList;
import java.util.List;

import com.you.Voxely.Mesh.Mesh;

public class ChunkBuilder {
    public static Mesh GenerateChunkMesh(Chunk chunk){
        List<Float> verticesList = new ArrayList<>();
        List<Integer> indicesList = new ArrayList<>();
        List<Float> colorsList = new ArrayList<>();

        float unit = 0.5f;
        int vertexCounter = 0;
        float[] whiteColor = {1f, 1f, 1f};
        for(int x = 0; x < Chunk.SIZE; x++){
            for(int z = 0; z < Chunk.SIZE; z++){
                for(int y = 0; y < Chunk.SIZE; y++){
                    if(!chunk.IsSolid(x, y, z)) continue;

                    float worldX = chunk.GetPosition().x + x;
                    float worldY = chunk.GetPosition().y + y;
                    float worldZ = chunk.GetPosition().z + z;
                    
                    // FRONT
                    if(!chunk.IsSolid(x, y, z+1)){
                        AddVertex(verticesList, worldX + unit, worldY + unit, worldZ + unit);
                        AddVertex(verticesList, worldX + unit, worldY - unit, worldZ + unit);
                        AddVertex(verticesList, worldX - unit, worldY - unit, worldZ + unit);
                        AddVertex(verticesList, worldX - unit, worldY + unit, worldZ + unit);
                        AddIndices(indicesList, vertexCounter);
                        AddColors(colorsList, new float[]{0, 0, 1});

                        vertexCounter += 4;
                    }


                    // BACK
                    if(!chunk.IsSolid(x, y, z-1)){
                        AddVertex(verticesList, worldX + unit, worldY + unit, worldZ - unit);
                        AddVertex(verticesList, worldX + unit, worldY - unit, worldZ - unit);
                        AddVertex(verticesList, worldX - unit, worldY - unit, worldZ - unit);
                        AddVertex(verticesList, worldX - unit, worldY + unit, worldZ - unit);
                        AddIndices(indicesList, vertexCounter);
                        AddColors(colorsList, new float[]{0, 0, 0.5f});

                        vertexCounter += 4;
                    }

                    // RIGHT
                    if(!chunk.IsSolid(x+1, y, z)){
                        AddVertex(verticesList, worldX + unit, worldY + unit, worldZ + unit);
                        AddVertex(verticesList, worldX + unit, worldY - unit, worldZ + unit);
                        AddVertex(verticesList, worldX + unit, worldY - unit, worldZ - unit);
                        AddVertex(verticesList, worldX + unit, worldY + unit, worldZ - unit);
                        AddIndices(indicesList, vertexCounter);
                        AddColors(colorsList, new float[]{1, 0, 0});

                        vertexCounter += 4;
                    }

                    if(!chunk.IsSolid(x-1, y, z)){
                        AddVertex(verticesList, worldX - unit, worldY + unit, worldZ + unit);
                        AddVertex(verticesList, worldX - unit, worldY - unit, worldZ + unit);
                        AddVertex(verticesList, worldX - unit, worldY - unit, worldZ - unit);
                        AddVertex(verticesList, worldX - unit, worldY + unit, worldZ - unit);
                        AddIndices(indicesList, vertexCounter);
                        AddColors(colorsList, new float[]{0.5f, 0, 0});

                        vertexCounter += 4;
                    }

                    if(!chunk.IsSolid(x, y+1, z)){
                        AddVertex(verticesList, worldX + unit, worldY + unit, worldZ + unit);
                        AddVertex(verticesList, worldX - unit, worldY + unit, worldZ + unit);
                        AddVertex(verticesList, worldX - unit, worldY + unit, worldZ - unit);
                        AddVertex(verticesList, worldX + unit, worldY + unit, worldZ - unit);
                        AddIndices(indicesList, vertexCounter);
                        AddColors(colorsList, new float[]{0, 1, 0});

                        vertexCounter += 4;
                    }

                    if(!chunk.IsSolid(x, y-1, z)){
                        AddVertex(verticesList, worldX + unit, worldY - unit, worldZ + unit);
                        AddVertex(verticesList, worldX - unit, worldY - unit, worldZ + unit);
                        AddVertex(verticesList, worldX - unit, worldY - unit, worldZ - unit);
                        AddVertex(verticesList, worldX + unit, worldY - unit, worldZ - unit);
                        AddIndices(indicesList, vertexCounter);
                        AddColors(colorsList, new float[]{0, 0.5f, 0});

                        vertexCounter += 4;
                    }


                }
            }
        }


        float[] finalVertices = ConvertFloatListToArray(verticesList);
        int[] finalIndices = ConvertIntListToArray(indicesList);
        float[] finalColors = ConvertFloatListToArray(colorsList);
        return new Mesh(chunk.GetPosition(), finalVertices, finalIndices, finalColors);
    }

    private static void AddVertex(List<Float> list, float x, float y, float z){
        list.add(x); list.add(y); list.add(z);
    }

    private static void AddIndices(List<Integer> list, int startIdx){
        list.add(startIdx); list.add(startIdx+1); list.add(startIdx+2);
        list.add(startIdx); list.add(startIdx+2); list.add(startIdx+3);
    }

    private static void AddColors(List<Float> list, float[] rgb) {
        for(int i = 0; i < 4; i++) {
            list.add(rgb[0]); list.add(rgb[1]); list.add(rgb[2]);
        }
    }

    private static float[] ConvertFloatListToArray(List<Float> list){
        float[] arr = new float[list.size()];
        for(int i = 0; i < list.size(); i++) arr[i] = list.get(i);
        return arr;
    }


    private static int[] ConvertIntListToArray(List<Integer> list){
        int[] arr = new int[list.size()];
        for(int i = 0; i < list.size(); i++) arr[i] = list.get(i);
        return arr;
    }
}
