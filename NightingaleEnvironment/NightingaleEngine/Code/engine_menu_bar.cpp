#include "pch.h"
#include "engine_menu_bar.h"
#include "engine_functions.h"

void EngineMenuBar::render_update()
{
    ImGui::BeginMainMenuBar();


    if (ImGui::Button("Console")) {
    }

    if (ImGui::Button("Render Panel")) {
        EngineFunctions::ExecuteCommand("render_panel");
    }
    if (ImGui::Button("Physics Control")) {
        EngineFunctions::ExecuteCommand("physics_control");
    }
    

    if (ImGui::BeginMenu("Browsers")) {
        if (ImGui::Button("Shader Browser")) {
            EngineFunctions::ExecuteCommand("shader_browser");
        }
        
        
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Advanced")) {
        if (ImGui::Button("Lag Simulator")) {
            EngineFunctions::ExecuteCommand("lag_simulator");
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
