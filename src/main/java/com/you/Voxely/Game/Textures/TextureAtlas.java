package com.you.Voxely.Game.Textures;

import org.lwjgl.stb.STBImage;
import static org.lwjgl.opengl.GL11.*;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import org.lwjgl.system.MemoryStack;

public class TextureAtlas{
    public final int width = 1024;
    public final int height = 1024;

    public int id;

    public TextureAtlas(){
        Initialize();
    }

    private void Initialize(){
        id = glGenTextures();
        glBindTexture(GL_TEXTURE_2D, id);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (ByteBuffer)null);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    public void InsertTexture(Texture texture, int xOffset, int yOffset){
        try(MemoryStack stack = MemoryStack.stackPush()){
            IntBuffer w = stack.mallocInt(1);
            IntBuffer h = stack.mallocInt(1);
            IntBuffer channels = stack.mallocInt(1);

            STBImage.stbi_set_flip_vertically_on_load(true);
            ByteBuffer pixels = STBImage.stbi_load(texture.path, w, h, channels, 4);

            if(pixels == null){
                throw new RuntimeException("Failed to load texture file: " + STBImage.stbi_failure_reason());
            }
            
            int width = w.get(0);
            int height = h.get(0);

            glBindTexture(GL_TEXTURE_2D, id);

            glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

            STBImage.stbi_image_free(pixels);
        }
    }
}
