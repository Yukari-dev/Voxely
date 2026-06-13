package com.you.Voxely.Game.GUI;

import java.util.ArrayList;
import java.util.List;

import imgui.ImGui;

public class ImGuiManager {
    private final List<GuiWindow> windows;

    public ImGuiManager(){
        windows = new ArrayList<>();
    }

    public void RegisterWindow(GuiWindow window){
        windows.add(window);
    }

    public void RenderAll(){
        for(GuiWindow window : windows){
            window.Render();
        }
    }

}
