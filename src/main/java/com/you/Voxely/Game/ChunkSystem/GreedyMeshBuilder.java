package com.you.Voxely.Game.ChunkSystem;

import java.util.ArrayList;
import java.util.List;

import com.you.Voxely.Mesh.Mesh;

public class GreedyMeshBuilder {
    public static  Mesh GenerateGreedyMesh(Chunk chunk, List<Float> verticesList){
        List<Float> newVertices = new ArrayList<>();
        List<Integer> newIndices = new ArrayList<>();
        List<Float> newColors = new ArrayList<>();
        int vertexCounter = 0;
        int targetBlockType = 1;
        float unit = 0.5f;
        int topY = Chunk.SIZE-1;
        boolean[][] mask = GenerateMask(chunk, targetBlockType);

        for(int x = 0 ;x < Chunk.SIZE; x++){
            for(int z = 0; z < Chunk.SIZE; z++){
                if(!mask[x][z]) continue;

                int startX = x;
                int startZ = z;
                int width = 1;

                while(startX + width < Chunk.SIZE && mask[startX+width][z]){
                    width++;
                }

                int length = 1;
                boolean canExpand = true;
                while(startZ + length < Chunk.SIZE && canExpand){
                    for(int w = 0; w < width; w++){
                        if(!mask[startX+w][startZ+length]){
                            canExpand = false;
                            break;
                        }
                    }
                    if(canExpand){
                        length++;
                    }
                }

                int endX = startX+width;
                int endZ = startZ+length;

                AddVertex(newVertices, startX - unit, topY + unit, startZ - unit); 
                AddVertex(newVertices, startX - unit, topY + unit, endZ - unit);
                AddVertex(newVertices, endX - unit,   topY + unit, endZ - unit);
                AddVertex(newVertices, endX - unit,   topY + unit, startZ - unit);

                AddIndices(newIndices, vertexCounter);
                AddColors(newColors, new float[]{0, 1, 0});
                vertexCounter+=4;

                for(int l = 0; l < length; l++){
                    for(int w = 0; w < width; w++){
                        mask[startX+w][startZ+l] = false;
                    }
                }
            }
        }

        float[] vertices = ConvertFloatListToArray(newVertices);
        int[] indices = ConvertIntListToArray(newIndices);
        float[] colors = ConvertFloatListToArray(newColors);
        return new Mesh(chunk.GetPosition(), vertices, indices, colors);
    }

    private static boolean[][] GenerateMask(Chunk chunk, int targetBlockType){
        boolean[][] mask = new boolean[Chunk.SIZE][Chunk.SIZE];
        for(int x = 0; x < Chunk.SIZE; x++){
            for(int z = 0; z < Chunk.SIZE; z++){
                if(chunk.IsBlockType(x, chunk.SIZE-1, z, targetBlockType)){
                    mask[x][z] = true;
                }
            }
        }
        return mask;
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
