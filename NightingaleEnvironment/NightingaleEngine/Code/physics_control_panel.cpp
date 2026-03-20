#include "pch.h"
#include "physics_control_panel.h"
#include "ngrender.h"
#include "engine_functions.h"
#include "renderer.h"
#include "physics_debug_render_pass.h"

void PhysicsControlPanel::render_update()
{
    bool bRemainOpen{ true };

    ImGui::Begin("Physics Control", &bRemainOpen);

    Physics& physics = EngineFunctions::physics();

    bool bPhysicsActive = physics.isActive();
    if (ImGui::Checkbox("Physics Active", &bPhysicsActive)) {
        physics.setActive(bPhysicsActive);
    }

    int updatesPerSecond = physics.getUpdatesPerSecond();
    if (ImGui::InputInt("Updates Per Second", &updatesPerSecond, 1)); {
        if (updatesPerSecond < 1) {
            updatesPerSecond = 1;
        }
        if (updatesPerSecond != physics.getUpdatesPerSecond()) {
            physics.setTargetUpdateRate(updatesPerSecond);
        }
    }

    ImGui::Text("Accumulated Time: %f", physics.getAccumulatedTime());
    ImGui::Text("Update Rate: %f", physics.getUpdateRate());

    bool bDiscardUnusedTime = physics.getDiscardUnusedTime();
    if (ImGui::Checkbox("Discard Unused Time", &bDiscardUnusedTime)) {
        EngineFunctions::physics().setDiscardUnusedTime(bDiscardUnusedTime);
    }

    bool bInterpolateBetweenFrames = physics.getInterpolateBetweenFrames();
    if (ImGui::Checkbox("Interpolate Between Frames", &bInterpolateBetweenFrames)) {
        EngineFunctions::physics().setInterpolateBetweenFrames(bDiscardUnusedTime);
    }

    int maxUpdatesPerFrame = EngineFunctions::physics().getMaxUpdatesPerFrame();
    if (ImGui::InputInt("Max Updates Per Frame", &maxUpdatesPerFrame, 1)); {
        EngineFunctions::physics().setMaxUpdatesPerFrame(maxUpdatesPerFrame);
    }

    ImGui::Separator();
    
    PhysicsDebugRenderPass* pDebugPass = findDebugPass();
    if (pDebugPass != nullptr) {
        pDebugPass->debugUIFunction();
    }
    else {
        if (ImGui::Button("Create a Debug Render Pass")) {
            pDebugPass = new PhysicsDebugRenderPass(EngineFunctions::physics());
            EngineFunctions::Renderer().registerRenderPass(pDebugPass);
        }
    }

    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

PhysicsDebugRenderPass* PhysicsControlPanel::findDebugPass()
{
    PhysicsDebugRenderPass* pDebugPass{ nullptr };
    vector<RenderPass*> const& vActiveRenderPasses = EngineFunctions::Renderer().getActiveRenderPasses();
    for (RenderPass* pRenderPass : vActiveRenderPasses) {
        pDebugPass = dynamic_cast<PhysicsDebugRenderPass*>(pRenderPass);
        if (pDebugPass != nullptr) {
            break;
        }
    }

    return pDebugPass;
}
