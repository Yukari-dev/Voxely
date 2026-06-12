package com.you.Voxely.Game.ChunkSystem;

import org.joml.Vector3f;

public class Chunk {
    public static final int SIZE = 16;
    public static final int HEIGHT = 256;
    
    private int[][][] blocks = new int[SIZE][HEIGHT][SIZE];

    private Vector3f position;

    public Chunk(Vector3f position){
        this.position = position;
        GenerateDummyTerrain();
    }

    private void GenerateDummyTerrain(){
        for(int x = 0; x < SIZE; x++){
            for(int z = 0; z < SIZE; z++){
                for(int y = 0; y < HEIGHT; y++){
                    blocks[x][y][z] = 1;
                }
            }
        }
    }

    public void ClearBlocks(){
        blocks = new int[SIZE][HEIGHT][SIZE];
    }

    public void SetBlockType(int x, int y, int z, int blockType){
        blocks[x][y][z] = blockType;
    }

    public boolean IsSolid(int x, int y, int z){
        if(IsBlockOutOfBondaries(x, y, z)) return false;
        return blocks[x][y][z] != 0;
    }

    private boolean IsBlockOutOfBondaries(int x, int y, int z){
        return x < 0 || x>=SIZE || y < 0 || y>=HEIGHT || z < 0 || z>=SIZE;
    }

    public Vector3f GetPosition(){
        return position;
    }

    public boolean IsBlockType(int x, int y, int z, int type){
        if(IsBlockOutOfBondaries(x, y, z)) return false;
        return blocks[x][y][z] == type;
    }
}
