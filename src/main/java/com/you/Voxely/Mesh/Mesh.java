package com.you.Voxely.Mesh;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import org.joml.Matrix4f;
import org.joml.Vector3f;
import org.lwjgl.system.MemoryUtil;

import com.you.Voxely.Renderer.UniformBlock;

import static org.lwjgl.opengl.GL30.*;

public class Mesh {
    private int vao, vbo, ebo;

    private Vector3f position;

    private float[] vertices;
    private int[] indices;
    private VertexLayout layout;


    public Mesh(Vector3f position, float[] vertices, int[] indices, VertexLayout layout){
        this.position = position;
        this.vertices = vertices;
        this.indices = indices;
        this.layout = layout;
        Construct();
    }

    public void Construct(){
        vao = CreateVertexArrayBuffer();
        vbo = CreateBuffer(GL_ARRAY_BUFFER, vertices);
        ebo = CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

        layout.Apply();   
        glBindVertexArray(0);
    }

    public void Draw(){
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.length, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    public Matrix4f GetModelMatrix(){
        return new Matrix4f().identity().translate(position).scale(1);
    }

    private int CreateVertexArrayBuffer(){
        int vao = glGenVertexArrays();
        glBindVertexArray(vao);
        return vao;
    }

    private int CreateBuffer(int type, float[] data){
        int buf = glGenBuffers();
        glBindBuffer(type, buf);
        FloatBuffer buffer = MemoryUtil.memAllocFloat(data.length);
        buffer.put(data).flip();
        glBufferData(type, buffer, GL_STATIC_DRAW);
        return buf;
    }

    private int CreateBuffer(int type, int[] data){
        int buf = glGenBuffers();
        glBindBuffer(type, buf);
        IntBuffer buffer = MemoryUtil.memAllocInt(data.length);
        buffer.put(data).flip();
        glBufferData(type, buffer, GL_STATIC_DRAW);
        return buf;
    }

}
