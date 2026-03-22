#include "pch.h"
#include "engine_menu_bar.h"
#include "engine_functions.h"

void EngineMenuBar::render_update()
{
    ImGui::BeginMainMenuBar();


    if (ImGui::Button("Console")) {
        EngineFunctions::ExecuteCommand("console");
    }

    if (ImGui::Button("Render Panel")) {
        EngineFunctions::ExecuteCommand("render_panel");
    }
    if (ImGui::Button("Physics Control")) {
        EngineFunctions::ExecuteCommand("physics_control");
    }
    if (ImGui::Button("Scene Hierarchy")) {
        EngineFunctions::ExecuteCommand("scene_hierarchy");
    }
    if (ImGui::Button("Inspector")) {
        EngineFunctions::ExecuteCommand("inspector");
    }
    

    if (ImGui::BeginMenu("Browsers")) {
        if (ImGui::Button("Shader Browser")) {
            EngineFunctions::ExecuteCommand("shader_browser");
        }

        if (ImGui::Button("Mesh Browser")) {
            EngineFunctions::ExecuteCommand("mesh_browser");
        }
        
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Advanced")) {
        if (ImGui::Button("Lag Simulator")) {
            EngineFunctions::ExecuteCommand("lag_simulator");
        }

        if (ImGui::Button("ImGUI Demo Window")) {
            EngineFunctions::ExecuteCommand("imgui_demo");
        }

        if (ImGui::Button("File Saving Panel")) {
            EngineFunctions::ExecuteCommand("file_saving_panel");
        }

        if (ImGui::Button("Close App")) {
            EngineFunctions::ExecuteCommand("quit");
        }

        ImGui::EndMenu();
    }


    if (ImGui::Button("Close")) {
        EngineFunctions::ExecuteCommand("engine_menu false");
    }

    ImGui::EndMainMenuBar();
}

void ImGUIDemoPanel::render_update()
{
    ImGui::ShowDemoWindow();
}
