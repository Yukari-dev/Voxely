package com.you.Voxely.Game.Textures;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL30.*;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import org.lwjgl.stb.STBImage;
import org.lwjgl.system.MemoryStack;

public class TextureLoader {
    public static Texture Load(String texturePath){
        try(MemoryStack memory = MemoryStack.stackPush()){
            IntBuffer w = memory.callocInt(1);
            IntBuffer h = memory.callocInt(1);
            IntBuffer channels = memory.callocInt(1);

            STBImage.stbi_set_flip_vertically_on_load(true);
            
            ByteBuffer pixels = STBImage.stbi_load(texturePath, w, h, channels, 4);
            if(pixels == null){
                throw new RuntimeException("Failed to load texture file: " + STBImage.stbi_failure_reason());
            }

            int width = w.get(0);
            int height = h.get(0);
            int id = glGenTextures();
            glBindTexture(GL_TEXTURE_2D, id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
            STBImage.stbi_image_free(pixels);

            glBindTexture(GL_TEXTURE_2D, id);

            Texture texture = new Texture(texturePath, id, width, height);
            return texture;
        }
    }
}
