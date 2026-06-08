package com.you.Voxely.Game;

import imgui.ImGui;
import imgui.gl3.ImGuiImplGl3;
import imgui.glfw.ImGuiImplGlfw;

public class VoxelyUI {
    private final ImGuiImplGlfw imGuiGlfw = new ImGuiImplGlfw();
    private final ImGuiImplGl3 imGuiGl3 = new ImGuiImplGl3();

    public void Initialize(long glwfWindowHandle){
        ImGui.createContext();

        imGuiGlfw.init(glwfWindowHandle, true);
        imGuiGl3.init("#version 330");
    }

    public void StartFrame() {
        imGuiGlfw.newFrame();
        ImGui.newFrame();
    }

    public void EndFrame(){
        ImGui.render();
        imGuiGl3.renderDrawData(ImGui.getDrawData());
    }

    public void Destroy(){
        imGuiGl3.dispose();
        imGuiGlfw.dispose();
        ImGui.destroyContext();
    }

}
