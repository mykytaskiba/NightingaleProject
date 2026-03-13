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



    if (ImGui::Button("Close")) {
        EngineFunctions::close_app();
    }

    ImGui::EndMainMenuBar();
}
