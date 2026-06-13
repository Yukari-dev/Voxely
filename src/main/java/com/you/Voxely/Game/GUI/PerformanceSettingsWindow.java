package com.you.Voxely.Game.GUI;

import com.you.Voxely.Engine.Window;
import com.you.Voxely.Time.Time;

import imgui.ImGui;
import imgui.type.ImBoolean;

public class PerformanceSettingsWindow implements GuiWindow{
    private Window window;

    private ImBoolean syncEnability = new ImBoolean(false);


    public PerformanceSettingsWindow(Window window){
        this.window = window;
    }

    @Override
    public String GetName() {
        return "Performance Window";
    }

    @Override
    public void Render() {
        ImGui.begin("Performance");

        ImGui.text(String.format("Game Frame Rate: %.1f FPS", Time.GetFPS()));
        ImGui.text(String.format("Frame Time: %.3f ms/Frame", 1000.0f/Time.GetFPS()));
        ImGui.text(String.format("Delta Time: %.6f ms/Frame", Time.GetDeltaTime()));

        if(ImGui.checkbox("Sync", syncEnability)){
            window.SetSyncEnablility(syncEnability.get());
        }

        ImGui.separator();

        long maxMemory = Runtime.getRuntime().maxMemory() / 1024 / 1024;
        long allocatedMemory = Runtime.getRuntime().totalMemory() / 1024 / 1024;
        long freeMemory = Runtime.getRuntime().freeMemory() / 1024 / 1024;

        ImGui.text("JVM Heap Memory Allocation:");
        ImGui.text("  Max Capacity Available: " + maxMemory + " MB");
        ImGui.text("  Allocated Space: " + allocatedMemory + " MB");
        ImGui.text("  Unused Buffer Space: " + freeMemory + " MB");

        ImGui.end();
    }
    
}
