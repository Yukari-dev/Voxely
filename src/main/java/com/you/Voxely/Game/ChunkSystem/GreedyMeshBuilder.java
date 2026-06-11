package com.you.Voxely.Game.ChunkSystem;

import java.util.ArrayList;
import java.util.List;

import com.you.Voxely.Mesh.Mesh;

public class GreedyMeshBuilder {

    public static Mesh GenerateGreedyMesh(World world, Chunk chunk) {
        List<Float> newVertices = new ArrayList<>();
        List<Integer> newIndices = new ArrayList<>();
        List<Float> newColors = new ArrayList<>();
        int vertexCounter = 0;
        float unit = 0.5f;

        int chunkX = (int) chunk.GetPosition().x;
        int chunkY = (int) chunk.GetPosition().y;
        int chunkZ = (int) chunk.GetPosition().z;

        int maskSize = Chunk.SIZE + 1;

        for (int y = 0; y <= Chunk.SIZE; y++) {
            boolean[][] topMask = new boolean[maskSize][maskSize];
            boolean[][] bottomMask = new boolean[maskSize][maskSize];
            int globalY = chunkY + y;

            for (int x = 0; x <= Chunk.SIZE; x++) {
                for (int z = 0; z <= Chunk.SIZE; z++) {
                    int globalX = chunkX + x;
                    int globalZ = chunkZ + z;

                    boolean currentSolid = world.IsBlockSolidAt(globalX, globalY, globalZ);
                    boolean belowSolid   = world.IsBlockSolidAt(globalX, globalY - 1, globalZ);

                    if (y > 0 && !currentSolid && belowSolid) {
                        topMask[x][z] = true;
                    }
                    if (y < Chunk.SIZE && currentSolid && !belowSolid) {
                        bottomMask[x][z] = true;
                    }
                }
            }

            if (y > 0) vertexCounter = ProcessMaskY(topMask, y - 1, unit, true, newVertices, newIndices, newColors, vertexCounter);
            if (y < Chunk.SIZE) vertexCounter = ProcessMaskY(bottomMask, y, unit, false, newVertices, newIndices, newColors, vertexCounter);
        }

        for (int z = 0; z <= Chunk.SIZE; z++) {
            boolean[][] frontMask = new boolean[maskSize][maskSize];
            boolean[][] backMask = new boolean[maskSize][maskSize];
            int globalZ = chunkZ + z;

            for (int x = 0; x <= Chunk.SIZE; x++) {
                for (int y = 0; y <= Chunk.SIZE; y++) {
                    int globalX = chunkX + x;
                    int globalY = chunkY + y;

                    boolean currentSolid = world.IsBlockSolidAt(globalX, globalY, globalZ);
                    boolean behindSolid  = world.IsBlockSolidAt(globalX, globalY, globalZ - 1);

                    if (z > 0 && !currentSolid && behindSolid) {
                        frontMask[x][y] = true;
                    }
                    if (z < Chunk.SIZE && currentSolid && !behindSolid) {
                        backMask[x][y] = true;
                    }
                }
            }

            if (z > 0) vertexCounter = ProcessMaskZ(frontMask, z - 1, unit, true, newVertices, newIndices, newColors, vertexCounter);
            if (z < Chunk.SIZE) vertexCounter = ProcessMaskZ(backMask, z, unit, false, newVertices, newIndices, newColors, vertexCounter);
        }

        for (int x = 0; x <= Chunk.SIZE; x++) {
            boolean[][] rightMask = new boolean[maskSize][maskSize];
            boolean[][] leftMask = new boolean[maskSize][maskSize];
            int globalX = chunkX + x;

            for (int z = 0; z <= Chunk.SIZE; z++) {
                for (int y = 0; y <= Chunk.SIZE; y++) {
                    int globalZ = chunkZ + z;
                    int globalY = chunkY + y;

                    boolean currentSolid = world.IsBlockSolidAt(globalX, globalY, globalZ);
                    boolean leftSolid    = world.IsBlockSolidAt(globalX - 1, globalY, globalZ);

                    if (x > 0 && !currentSolid && leftSolid) {
                        rightMask[z][y] = true;
                    }
                    if (x < Chunk.SIZE && currentSolid && !leftSolid) {
                        leftMask[z][y] = true;
                    }
                }
            }

            if (x > 0) vertexCounter = ProcessMaskX(rightMask, x - 1, unit, true, newVertices, newIndices, newColors, vertexCounter);
            if (x < Chunk.SIZE) vertexCounter = ProcessMaskX(leftMask, x, unit, false, newVertices, newIndices, newColors, vertexCounter);
        }

        float[] vertices = ConvertFloatListToArray(newVertices);
        int[] indices = ConvertIntListToArray(newIndices);
        float[] colors = ConvertFloatListToArray(newColors);
        return new Mesh(chunk.GetPosition(), vertices, indices, colors);
    }

    private static int ProcessMaskY(boolean[][] mask, int y, float unit, boolean isTop, List<Float> vertices, List<Integer> indices, List<Float> colors, int vertexCounter) {
        float[] faceColor = isTop ? new float[]{0, 1, 0} : new float[]{0, 0.5f, 0};
        float yPos = y + (isTop ? unit : -unit);
        int limit = mask.length;

        for (int x = 0; x < Chunk.SIZE; x++) {
            for (int z = 0; z < Chunk.SIZE; z++) {
                if (!mask[x][z]) continue;

                int width = 1;
                while (x + width < Chunk.SIZE && mask[x + width][z]) width++;

                int length = 1;
                boolean canExpand = true;
                while (z + length < Chunk.SIZE && canExpand) {
                    for (int w = 0; w < width; w++) {
                        if (!mask[x + w][z + length]) { canExpand = false; break; }
                    }
                    if (canExpand) length++;
                }

                int endX = x + width;
                int endZ = z + length;

                if (isTop) {
                    AddVertex(vertices, x - unit, yPos, z - unit);
                    AddVertex(vertices, x - unit, yPos, endZ - unit);
                    AddVertex(vertices, endX - unit, yPos, endZ - unit);
                    AddVertex(vertices, endX - unit, yPos, z - unit);
                } else {
                    AddVertex(vertices, x - unit, yPos, z - unit);
                    AddVertex(vertices, endX - unit, yPos, z - unit);
                    AddVertex(vertices, endX - unit, yPos, endZ - unit);
                    AddVertex(vertices, x - unit, yPos, endZ - unit);
                }

                AddIndices(indices, vertexCounter);
                AddColors(colors, faceColor);
                vertexCounter += 4;

                ClearMaskArea(mask, x, z, width, length);
            }
        }
        return vertexCounter;
    }

    private static int ProcessMaskZ(boolean[][] mask, int z, float unit, boolean isFront, List<Float> vertices, List<Integer> indices, List<Float> colors, int vertexCounter) {
        float[] faceColor = isFront ? new float[]{0, 0, 1} : new float[]{0, 0, 0.5f};
        float zPos = z + (isFront ? unit : -unit);

        for (int x = 0; x < Chunk.SIZE; x++) {
            for (int y = 0; y < Chunk.SIZE; y++) {
                if (!mask[x][y]) continue;

                int width = 1;
                while (x + width < Chunk.SIZE && mask[x + width][y]) width++;

                int height = 1;
                boolean canExpand = true;
                while (y + height < Chunk.SIZE && canExpand) {
                    for (int w = 0; w < width; w++) {
                        if (!mask[x + w][y + height]) { canExpand = false; break; }
                    }
                    if (canExpand) height++;
                }

                int endX = x + width;
                int endY = y + height;

                if (isFront) {
                    AddVertex(vertices, endX - unit, y - unit, zPos);
                    AddVertex(vertices, endX - unit, endY - unit, zPos);
                    AddVertex(vertices, x - unit, endY - unit, zPos);
                    AddVertex(vertices, x - unit, y - unit, zPos);
                } else {
                    AddVertex(vertices, x - unit, y - unit, zPos);
                    AddVertex(vertices, x - unit, endY - unit, zPos);
                    AddVertex(vertices, endX - unit, endY - unit, zPos);
                    AddVertex(vertices, endX - unit, y - unit, zPos);
                }

                AddIndices(indices, vertexCounter);
                AddColors(colors, faceColor);
                vertexCounter += 4;

                ClearMaskArea(mask, x, y, width, height);
            }
        }
        return vertexCounter;
    }

    private static int ProcessMaskX(boolean[][] mask, int x, float unit, boolean isRight, List<Float> vertices, List<Integer> indices, List<Float> colors, int vertexCounter) {
        float[] faceColor = isRight ? new float[]{1, 0, 0} : new float[]{0.5f, 0, 0};
        float xPos = x + (isRight ? unit : -unit);

        for (int z = 0; z < Chunk.SIZE; z++) {
            for (int y = 0; y < Chunk.SIZE; y++) {
                if (!mask[z][y]) continue;

                int length = 1;
                while (z + length < Chunk.SIZE && mask[z + length][y]) length++;

                int height = 1;
                boolean canExpand = true;
                while (y + height < Chunk.SIZE && canExpand) {
                    for (int l = 0; l < length; l++) {
                        if (!mask[z + l][y + height]) { canExpand = false; break; }
                    }
                    if (canExpand) height++;
                }

                int endZ = z + length;
                int endY = y + height;

                if (isRight) {
                    AddVertex(vertices, xPos, y - unit, endZ - unit);
                    AddVertex(vertices, xPos, endY - unit, endZ - unit);
                    AddVertex(vertices, xPos, endY - unit, z - unit);
                    AddVertex(vertices, xPos, y - unit, z - unit);
                } else {
                    AddVertex(vertices, xPos, y - unit, z - unit);
                    AddVertex(vertices, xPos, endY - unit, z - unit);
                    AddVertex(vertices, xPos, endY - unit, endZ - unit);
                    AddVertex(vertices, xPos, y - unit, endZ - unit);
                }

                AddIndices(indices, vertexCounter);
                AddColors(colors, faceColor);
                vertexCounter += 4;

                ClearMaskArea(mask, z, y, length, height);
            }
        }
        return vertexCounter;
    }

    private static void ClearMaskArea(boolean[][] mask, int startU, int startV, int width, int length) {
        for (int l = 0; l < length; l++) {
            for (int w = 0; w < width; w++) {
                mask[startU + w][startV + l] = false;
            }
        }
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
