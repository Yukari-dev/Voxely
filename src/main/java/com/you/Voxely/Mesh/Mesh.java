package com.you.Voxely.Mesh;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import org.joml.Matrix4f;
import org.joml.Vector3f;
import org.lwjgl.system.MemoryUtil;

import com.you.Voxely.Game.Textures.Texture;
import com.you.Voxely.Renderer.Shader;

import static org.lwjgl.opengl.GL30.*;

public class Mesh {
    private int vao, posVBO, colVBO, UVsVBO, normVBO, ebo;

    private Vector3f position;

    private float[] vertices;
    private int[] indices;
    private float[] colors;
    private float[] uvs;
    private float[] normals;

    private Texture texture;

    public Mesh(Vector3f position, float[] vertices, int[] indices, float[] colors, float[] uvs, float[] normals, Texture texture){
        this.position = position;
        this.vertices = vertices;
        this.indices = indices;
        this.colors = colors;
        this.uvs = uvs;
        this.normals = normals;
        this.texture = texture;
        Construct();
    }

    public void Construct(){
        vao = CreateVertexArrayBuffer();

        posVBO = CreateBuffer(GL_ARRAY_BUFFER, vertices);
        VertexLayout posLayout = new VertexLayout().SetStartLocation(0).Add(VertexLayout.AttributeType.POSITION);
        posLayout.Apply();

        UVsVBO = CreateBuffer(GL_ARRAY_BUFFER, uvs);
        VertexLayout UVsLayout = new VertexLayout().SetStartLocation(1).Add(VertexLayout.AttributeType.UV);
        UVsLayout.Apply();

        colVBO = CreateBuffer(GL_ARRAY_BUFFER, colors);
        VertexLayout colLayout = new VertexLayout().SetStartLocation(2).Add(VertexLayout.AttributeType.COLOR);
        colLayout.Apply();

        normVBO = CreateBuffer(GL_ARRAY_BUFFER, normals);
        VertexLayout normLayout = new VertexLayout().SetStartLocation(3).Add(VertexLayout.AttributeType.NORMAL);
        normLayout.Apply();

        ebo = CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

        glBindVertexArray(0);
    }

    public void Draw(Shader shader){
        texture.Bind(shader);
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
        MemoryUtil.memFree(buffer);
        return buf;
    }

    private int CreateBuffer(int type, int[] data){
        int buf = glGenBuffers();
        glBindBuffer(type, buf);
        IntBuffer buffer = MemoryUtil.memAllocInt(data.length);
        buffer.put(data).flip();
        glBufferData(type, buffer, GL_STATIC_DRAW);
        MemoryUtil.memFree(buffer);
        return buf;
    }

    public void CleanUp(){
        glDeleteVertexArrays(vao); 
        
        glDeleteBuffers(new int[]{posVBO, colVBO, ebo}); 
    }

}
