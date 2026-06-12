package com.you.Voxely.Game.ChunkSystem;

import org.joml.Vector3f;

public class Chunk {
    public static final int SIZE = 16;
    public static final int HEIGHT = 256;
    
    private final short[] blocks;

    private Vector3f position;

    public Chunk(Vector3f position){
        this.position = position;
        blocks = new short[SIZE*HEIGHT*SIZE];
    }

    public void SetBlockType(int x, int y, int z, short blockType){
        blocks[GetIndex(x, y, z)] = blockType;
    }


    public short GetBlockType(int x, int y, int z){
        if(IsBlockOutOfBondaries(x, y, z)) return 0;
        return blocks[GetIndex(x, y, z)];
    }

    public boolean IsSolid(int x, int y, int z){
        if(IsBlockOutOfBondaries(x, y, z)) return false;
        return blocks[GetIndex(x, y, z)] != 0;
    }

    private boolean IsBlockOutOfBondaries(int x, int y, int z){
        return x < 0 || x>=SIZE || y < 0 || y>=HEIGHT || z < 0 || z>=SIZE;
    }

    public Vector3f GetPosition(){
        return position;
    }

    public boolean IsBlockType(int x, int y, int z, int type){
        if(IsBlockOutOfBondaries(x, y, z)) return false;
        return blocks[GetIndex(x, y, z)] == type;
    }

    private int GetIndex(int x, int y, int z){
        return (x*HEIGHT*SIZE)+(y*SIZE)+z;
    }
}
