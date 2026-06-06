package com.you.Voxely.Engine.Mesh;

import static org.lwjgl.opengl.GL11.GL_FLOAT;
import static org.lwjgl.opengl.GL30.*;

import java.util.ArrayList;
import java.util.List;

public class VertexLayout {
    public enum AttributeType{
        POSITION(3),
        NORMAL(3),
        UV(2),
        COLOR(4);

        public final int componentCount;
        AttributeType(int componentCount){
            this.componentCount = componentCount;
        }
    }

    private static class Attribute{
        AttributeType type;
        int location;
        int offset;

        Attribute(AttributeType type, int location, int offset){
            this.type = type;
            this.location = location;
            this.offset = offset;
        }
    }

    private final List<Attribute> attributes = new ArrayList<>(); 
    private int stride = 0;
    private int nextLocation = 0;
    
    public VertexLayout Add(AttributeType type){
        attributes.add(new Attribute(type, nextLocation, stride));
        stride += type.componentCount * Float.BYTES;
        nextLocation++;
        return this;
    }

    public void Apply(){
        for (Attribute attr : attributes) {
            glVertexAttribPointer(
                attr.location,
                attr.type.componentCount,
                GL_FLOAT,
                false,
                stride,
                attr.offset
            );
            glEnableVertexAttribArray(attr.location);
        }
    }

    public int GetStride()   { return stride; }
}
