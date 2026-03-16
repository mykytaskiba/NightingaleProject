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


    if (ImGui::Button("Set Preset: Simple Forward Render Pass")) {
        EngineFunctions::Renderer().clearAllRenderPasses();
        EngineFunctions::Renderer().registerRenderPass(new ForwardRenderPass());
    }

    float width = ImGui::GetContentRegionAvail().x;
    ImGui::BeginChild("RENDER_PASSES", ImVec2(width, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border, ImGuiWindowFlags_MenuBar);
    ImGui::Text("Render passes");

    vector<RenderPass*>& vActiveRenderPasses = EngineFunctions::Renderer().getActiveRenderPasses();

    uint passCount{ 0u };
    for (RenderPass* pRenderPass : vActiveRenderPasses) {
        if (pRenderPass == nullptr) {
            assert(false);
            continue;
        }

        float width = ImGui::GetContentRegionAvail().x;
        std::string idStr = "PASSNUM" + passCount++;
        ImGui::BeginChild(idStr.c_str(), ImVec2(width, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);

        bool bShowingPass = ImGui::CollapsingHeader(pRenderPass->getInfoName().c_str(), ImGuiTreeNodeFlags_AllowOverlap);
        ImGui::SameLine(0.0f, 30.0f);
        if (ImGui::Button("Move Up")) {
            if (*vActiveRenderPasses.begin() != pRenderPass) {
                auto it_found = std::find(vActiveRenderPasses.begin(), vActiveRenderPasses.end(), pRenderPass);
                auto it_prev = it_found - 1;
                std::swap(*it_found, *it_prev);

                ImGui::EndChild();
                break;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Move Down")) {

        }
        ImGui::SameLine();
        if (ImGui::Button("X")) {
            ImGui::EndChild();

            EngineFunctions::Renderer().removeRenderPass(pRenderPass);
            break;
        }


        if (ImGui::BeginPopupContextItem()) {
            ImGui::EndPopup();
        }
        

        if (bShowingPass) {
            pRenderPass->debugUIFunction();
        }

        ImGui::EndChild();
    }

    ImGui::EndChild();


    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

