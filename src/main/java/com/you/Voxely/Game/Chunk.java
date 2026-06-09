package com.you.Voxely.Game;

import org.joml.Vector3f;

public class Chunk {
    public static final int SIZE = 16;
    
    private byte[][][] blocks = new byte[SIZE][SIZE][SIZE];

    private Vector3f position;

    public Chunk(Vector3f position){
        this.position = position;
        GenerateDummyTerrain();
    }

    private void GenerateDummyTerrain(){
        for(int x = 0; x < SIZE; x++){
            for(int z = 0; z < SIZE; z++){
                for(int y = 0; y < SIZE; y++){
                    blocks[x][y][z] = 1;
                }
            }
        }
    }

    public boolean IsSolid(int x, int y, int z){
        if(IsBlockOutOfBondaries(x, y, z)) return false;
        return blocks[x][y][z] != 0;
    }

    private boolean IsBlockOutOfBondaries(int x, int y, int z){
        return x < 0 || x>=SIZE || y < 0 || y>=SIZE || z < 0 || z>=SIZE;
    }

    public Vector3f GetPosition(){
        return position;
    }
}
