package com.you.Voxely.Game.ChunkSystem;

import java.util.Objects;

public class ChunkPosition {
    public final int x, y, z;

    public ChunkPosition(int x, int y, int z){
        this.x = x;
        this.y = y;
        this.z = z;
    }

    @Override
    public boolean equals(Object o){
        if(this == o) return true;
        if(!(o instanceof ChunkPosition)) return false;
        ChunkPosition p = (ChunkPosition)o;
        return this.x == p.x && this.y == p.y && this.z == p.z;
    }

    @Override 
    public int hashCode(){
        return Objects.hash(x, y, z);
    }
}
