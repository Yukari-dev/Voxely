package com.you.Voxely.Game.Blocks;

import java.util.Map;

public class BlockDefinition {
    public int id;
    public String name;
    public boolean isSolid;
    public Map<String, String> textures;


    public String GetTextureForFace(String face){
        if(textures.containsKey(face)){
            return textures.get(face);
        }
        if(textures.containsKey("side") && (face.equals("side") ||face.equals("north") || face.equals("south") || face.equals("west") || face.equals("east"))){
            return textures.get("side");
        }
        return textures.getOrDefault("all", "UNKNOWN");
    }
}
