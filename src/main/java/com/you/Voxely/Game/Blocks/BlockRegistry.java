package com.you.Voxely.Game.Blocks;

import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.Type;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

public class BlockRegistry {
    private static final Map<String, BlockDefinition> registeryByName = new HashMap<>();
    private static final Map<Integer, BlockDefinition> registeryById = new HashMap<>();

    public static void LoadDefinitions(){
        Gson gson = new Gson();
        String dataPath = "src/main/resources/Data/Blocks/Blocks.json";
        try(FileReader reader = new FileReader(dataPath)){
            Type listType = new TypeToken<List<BlockDefinition>>(){}.getType();
            List<BlockDefinition> blocks = gson.fromJson(reader, listType);

            for(BlockDefinition block : blocks){
                registeryById.put(block.id, block);
                registeryByName.put(block.name, block);
                System.out.println("Registered block: " + block.name + " (ID: " + block.id + ")");
            }
        } catch(IOException e){
            System.err.println("Failed to read block configuration file.");
            e.printStackTrace();
        }
    }

    public static BlockDefinition GetBlock(int id){
        return registeryById.get(id);
    }

    public static BlockDefinition GetBlock(String name){
        return registeryByName.get(name);
    }

}
