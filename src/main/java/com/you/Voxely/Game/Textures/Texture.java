package com.you.Voxely.Game.Textures;

import static org.lwjgl.opengl.GL11.GL_TEXTURE_2D;
import static org.lwjgl.opengl.GL11.glBindTexture;
import static org.lwjgl.opengl.GL13.*;

import com.you.Voxely.Renderer.Shader;


public class Texture {
    public final String path;
    public final int id;

    public final int width;
    public final int height;

    public Texture(String path, int id, int width, int height){
        this.path = path;
        this.id = id;
        this.width = width;
        this.height = height;
    }

    public void Bind(Shader shader){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);
        shader.SetInt("textureSampler", 0);
    }
}
