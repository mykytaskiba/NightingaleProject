#include "pch.h"
#include "render_debug_panel.h"
#include "engine_functions.h"
#include "engine_internals.h"
#include "forward_render_pass.h"
#include "physics_debug_render_pass.h"

void RenderDebugPanel::render_update()
{

    bool bRemainOpen{ true };

    ImGui::Begin("Render Debug", &bRemainOpen);

    float fps = 1.0f / EngineFunctions::delta_time();
    string fpsStr = "FPS: " + std::to_string(fps);
    ImGui::Text(fpsStr.c_str());
    
    int targetFrameRateVal = EngineFunctions::getTargetFramerateInt();
    bool bChanged = ImGui::InputInt("Target", &targetFrameRateVal, 10);
    if (bChanged) {
        if (targetFrameRateVal > 0) {
            EngineFunctions::SetTargetFramerate(targetFrameRateVal);
        }
    }
    float width = ImGui::GetContentRegionAvail().x;
    ImGui::BeginChild("RENDER_PASSES", ImVec2(width, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border, ImGuiWindowFlags_MenuBar);
    ImGui::Text("Render passes");

    vector<RenderPass*> const& vActiveRenderPasses = EngineFunctions::Renderer().getActiveRenderPasses();
    for (RenderPass* pRenderPass : vActiveRenderPasses) {
        render_pass_debug(pRenderPass);

    }

    ImGui::EndChild();


    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

void RenderDebugPanel::render_pass_debug(RenderPass* pRenderPass)
{
    pRenderPass->debugUIFunction();

    //ImGui::Text("Unknown render pass");
}
