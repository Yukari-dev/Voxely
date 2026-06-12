package com.you.Voxely.Game.Textures;

import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL30.*;

public class TextureAtlasRegistery {
    public static Map<Integer, Texture> loadedTextures = new HashMap<>();
    public static Map<String, TextureUVInfo> atlasCoordinates = new HashMap<>();

    public static TextureAtlas atlas = new TextureAtlas();

    private static String[] supportedExtensions = new String[]{"png", "jpeg", "jpg"};

    public static final int TEXTURE_SIZE = 16;

    public static class TextureUVInfo{
        public float uMin, vMin, uMax, vMax;
        public TextureUVInfo(int x, int y, int size, int atlasW, int atlasH) {
            this.uMin = (float) x / atlasW;
            this.vMin = (float) y / atlasH;
            this.uMax = (float) (x + size) / atlasW;
            this.vMax = (float) (y + size) / atlasH;
        }
    }

    public static void LoadTextures(){
        String realPath = String.format("%s", "src/main/resources/Textures/");
        Path path = Paths.get(realPath);

        int currentX = 0;
        int currentY = 0;
        
        try(DirectoryStream<Path> stream = Files.newDirectoryStream(path)){
            for(Path entry : stream){
                String fileName = entry.getFileName().toString();
                if(IsCorrectExtension(fileName)){
                    Texture texture = TextureLoader.Load(entry.toAbsolutePath().toString());
                    InsertInAtlas(texture, currentX, currentY);
                    
                    String textureName = fileName.substring(0, fileName.lastIndexOf("."));
                    System.out.println(textureName);

                    atlasCoordinates.put(textureName, new TextureUVInfo(currentX, currentY, TEXTURE_SIZE, atlas.width, atlas.height));

                    currentX += TEXTURE_SIZE;
                    if(currentX >= atlas.width){
                        currentX = 0;
                        currentY += TEXTURE_SIZE;
                    }

                    loadedTextures.put(texture.id, texture);
                }
            }
        } catch(IOException e){
            e.printStackTrace();
        }

        glBindTexture(GL_TEXTURE_2D, atlas.id);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    private static void InsertInAtlas(Texture texture, int xOffset, int yOffset){
        atlas.InsertTexture(texture, xOffset, yOffset);
    }

    public static TextureUVInfo GetUVInfo(String textureName) {
        TextureUVInfo UVInfo = atlasCoordinates.getOrDefault(textureName, atlasCoordinates.get("UNKNOWN"));
        return UVInfo;
    }

    private static boolean IsCorrectExtension(String fileName){
        String fileExtension = fileName.substring(fileName.lastIndexOf(".")+1);
        for(String supportedExtension : supportedExtensions){
            if(supportedExtension.compareTo(fileExtension) == 0){
                return true;
            }
        }
        return false;
    }

}
