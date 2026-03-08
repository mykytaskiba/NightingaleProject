#include "pch.h"
#include "render_debug_panel.h"
#include "engine_functions.h"

void RenderDebugPanel::render_update()
{

    bool bRemainOpen{ true };

    ImGui::Begin("Render Debug", &bRemainOpen);

    float fps = 1.0f / EngineFunctions::DeltaTime();
    string fpsStr = "FPS: " + std::to_string(fps);
    ImGui::Text(fpsStr.c_str());
    
    int targetFrameRateVal = EngineFunctions::getTargetFramerateInt();
    bool bChanged = ImGui::InputInt("Target:", &targetFrameRateVal, 10);
    if (bChanged) {
        if (targetFrameRateVal > 0) {
            EngineFunctions::SetTargetFramerate(targetFrameRateVal);
        }
    }


    ImGui::End();

    if (!bRemainOpen) {
        EngineFunctions::ExecuteCommand("set_render_debug false");
    }
}
