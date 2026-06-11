package com.you.Voxely.Game.ChunkSystem;

import org.joml.Vector3f;

public class MeshResultData {
    public final Vector3f chunkPosition;
    public final float[] vertices;
    public final int[] indices;
    public final float[] colors;

    public MeshResultData(Vector3f chunkPosition, float[] vertices, int[] indices, float[] colors){
        this.chunkPosition = chunkPosition;
        this.vertices = vertices;
        this.indices = indices;
        this.colors = colors;
    }
}