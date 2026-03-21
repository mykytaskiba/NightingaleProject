#include "pch.h"
#include "physics_control_panel.h"
#include "ngrender.h"
#include "engine_functions.h"
#include "renderer.h"
#include "physics_debug_render_pass.h"
#include "scene.h"
#include "camera_controller.h"
#include "render_mesh.h"
#include "asset_manager.h"

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
        EngineFunctions::physics().setInterpolateBetweenFrames(bInterpolateBetweenFrames);
    }

    int maxUpdatesPerFrame = EngineFunctions::physics().getMaxUpdatesPerFrame();
    if (ImGui::InputInt("Max Updates Per Frame", &maxUpdatesPerFrame, 1)); {
        EngineFunctions::physics().setMaxUpdatesPerFrame(maxUpdatesPerFrame);
    }

    float maxAccumulatedTime = physics.getMaxAccumulatedTime();
    if (ImGui::InputFloat("Max Accumulated Time", &maxAccumulatedTime)) {
        physics.setMaxAccumulatedTime(maxAccumulatedTime);
    }

    if (ImGui::Button("Test Case")) {
        setTestCase();
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

    ImGui::Separator();

    ImGui::Spacing();
    if (ImGui::TreeNodeEx("Step Simulation", ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
        if (ImGui::InputInt("Simulation Frame Count", &m_simulationFrames)) {}
        if (ImGui::Button("Simulate Frames")) {
            float frameToSimulationTime = ((float)m_simulationFrames) * physics.getUpdateRate();;
            forceSimulatePhysics(frameToSimulationTime);

        }

        if (ImGui::InputFloat("Simulation Step Time", &m_simulationTime)) {}
        if (ImGui::Button("Simulate Step")) {
            forceSimulatePhysics(m_simulationTime);
        }

        if (ImGui::InputFloat("Added Accumulation Time", &m_addedAccumulationTime)) {}
        if (ImGui::Button("Add Accumulation Time")) {
            physics.addAccumulatedTime(m_addedAccumulationTime);
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

void PhysicsControlPanel::setTestCase()
{
    EngineFunctions::physics().setActive(false);
    EngineFunctions::scene().clearScene();

    EngineFunctions::InstantiateGameObject<CameraController>();

    int ySize = 2;
    int xSize = 2;
    int zSize = 2;

    float offset = 3.0f;
    float cubeSize = 1.0f;

    float velocityFromCenter = 10.0f;
    Vector3 center = { 0,0,0 };

    for (int x = -xSize; x <= xSize; x++) {
        for (int z = -zSize; z <= zSize; z++) {
            for (int y = -ySize; y <= ySize; y++) {
                Vector3 position{ (float)x,(float)y,(float)z };
                position *= offset * cubeSize;

                GameObject* pGameObject = EngineFunctions::InstantiateGameObject<GameObject>();
                RenderMeshNode* pRenderNode = new RenderMeshNode();
                pRenderNode->setMesh(AssetManager<Mesh>::retrieve("sphere_mesh"));

                EngineFunctions::AssignRenderNode(pGameObject, pRenderNode);
                EngineFunctions::AttachPhysicsBody(pGameObject);

                PhysicsBody* pBody = pGameObject->getPhysicsBody();
                pBody->setLocalBox(AxisAlignedBox({ 0,0,0 }, { cubeSize,cubeSize,cubeSize }));
                pGameObject->getTransform().position = position;
                pBody->setVelocity((position - center).normalized() * velocityFromCenter);
                pBody->setGravity(true);

            }
        }
    }

    GameObject* pFloor = EngineFunctions::InstantiateGameObject<GameObject>();
    EngineFunctions::AttachPhysicsBody(pFloor);
    pFloor->getPhysicsBody()->setLocalBox(AxisAlignedBox({ 0.0f,0.0f,0.0f }, { 150.0f,0.25f,150.0f }));
    pFloor->getTransform().position = (Vector3(0, -10.0f, 0));

}

void PhysicsControlPanel::forceSimulatePhysics(float simulateTime)
{
    Physics& physics = EngineFunctions::physics();
    physics.setActive(true);
    uint currentMaxUpdatesPerFrame = physics.getMaxUpdatesPerFrame();
    physics.setMaxUpdatesPerFrame(100000u);
    physics.update(simulateTime);
    physics.setActive(false);
    physics.setMaxUpdatesPerFrame(currentMaxUpdatesPerFrame);
    EngineFunctions::scene().sync_gameobjects_to_physics();
}
