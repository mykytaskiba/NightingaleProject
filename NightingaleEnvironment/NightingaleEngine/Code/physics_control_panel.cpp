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
#include "property_menu.h"
#include "ngphys.h"

void PhysicsControlPanel::render_update()
{
    bool bRemainOpen{ true };

    ImGui::Begin("Physics Control", &bRemainOpen);

    Physics& physics = EngineFunctions::physics();

    PropertyMenu::render_update(physics);

    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::TreeNodeEx("Test Cases", ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
        if (ImGui::Button("Case Box On Box")) {
            caseBoxOnWeirdBox(false);
        }
        if (ImGui::Button("Case Box On 45Box")) {
            caseBoxOnWeirdBox(true);
        }
        if (ImGui::Button("Case Box On 45Plane")) {
            caseBoxOnWeirdBox(false, true);
        }
        if (ImGui::Button("Case Sphere On Sphere")) {
            caseSphereOnSphere();
        }
        if (ImGui::Button("Case Sphere Explosion")) {
            caseSphereExplosion();
        }
    }

    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::TreeNodeEx("Debug Rendering", ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
        PhysicsDebugRenderPass* pDebugPass = findDebugPass();
        if (pDebugPass != nullptr) {
            PropertyMenu::render_update(*pDebugPass);
        }
        else {
            if (ImGui::Button("Create a Debug Render Pass")) {
                pDebugPass = new PhysicsDebugRenderPass(EngineFunctions::physics());
                EngineFunctions::Renderer().registerRenderPass(pDebugPass);
            }
        }
    }

    ImGui::Text("Point Number: %i", physics.collisionPoints.size());
    if (ImGui::Button("Clear Accumulated Points")) {
        physics.collisionPoints.clear();
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

    ImGui::Text("Pairs generated %i", physics.getSpatialPairsCount());

    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

PhysicsDebugRenderPass* PhysicsControlPanel::findDebugPass()
{
    PhysicsDebugRenderPass* pDebugPass{ nullptr };
    std::vector<RenderPass*> const& vActiveRenderPasses = EngineFunctions::Renderer().getActiveRenderPasses();
    for (RenderPass* pRenderPass : vActiveRenderPasses) {
        pDebugPass = dynamic_cast<PhysicsDebugRenderPass*>(pRenderPass);
        if (pDebugPass != nullptr) {
            break;
        }
    }

    return pDebugPass;
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
    EngineFunctions::scene().syncObjectToPhysics();
}

void PhysicsControlPanel::caseSingleSphere()
{
    EngineFunctions::physics().setActive(false);
    EngineFunctions::scene().clearScene();

    EngineFunctions::InstantiateGameObject<CameraController>();

    Vector3 position{ 3.0f,10.0f,0.0f };

    GameObject* pGameObject = EngineFunctions::InstantiateGameObject<GameObject>();
    RenderMeshNode* pRenderNode = new RenderMeshNode();
    pRenderNode->setMesh(AssetManager<Mesh>::retrieve("sphere_mesh"));

    EngineFunctions::AssignRenderNode(pGameObject, pRenderNode);
    EngineFunctions::AttachPhysicsBody(pGameObject);

    PhysicsBody* pBody = pGameObject->getPhysicsBody();

    float localBoxSize = 2.0f;
    pBody->setLocalBox(AxisAlignedBox({ 0,0,0 }, { localBoxSize, localBoxSize, localBoxSize }));
    pGameObject->getTransform().position = position;
    //pBody->setVelocity({ 0.0f,0.0f,0.0f });
    pBody->setGravity(true);

    SphereShape* pSphereShape = new SphereShape();
    pSphereShape->setRadius(1.0f);
    pBody->setShape(pSphereShape);


    GameObject* pFloor = EngineFunctions::InstantiateGameObject<GameObject>();
    EngineFunctions::AttachPhysicsBody(pFloor);
    float floorBoxSize = 10.0f;
    pFloor->getPhysicsBody()->setLocalBox(AxisAlignedBox({ 0.0f,0.0f,0.0f }, { floorBoxSize, floorBoxSize, floorBoxSize }));
    pFloor->getTransform().position = (Vector3(0, -10.0f, 0));

    SphereShape* pFloorShape = new SphereShape();
    pFloorShape->setRadius(5.0f);
    pFloor->getPhysicsBody()->setShape(pFloorShape);
}

void PhysicsControlPanel::caseSphereOnSphere()
{
    EngineFunctions::physics().setActive(false);
    EngineFunctions::scene().clearScene();

    EngineFunctions::InstantiateGameObject<CameraController>();

    Vector3 position{ 3.0f,10.0f,0.0f };

    GameObject* pGameObject = EngineFunctions::InstantiateGameObject<GameObject>();
    RenderMeshNode* pRenderNode = new RenderMeshNode();
    pRenderNode->setMesh(AssetManager<Mesh>::retrieve("sphere_mesh"));

    EngineFunctions::AssignRenderNode(pGameObject, pRenderNode);
    EngineFunctions::AttachPhysicsBody(pGameObject);

    PhysicsBody* pBody = pGameObject->getPhysicsBody();

    float localBoxSize = 2.0f;
    pBody->setLocalBox(AxisAlignedBox({ 0,0,0 }, { localBoxSize, localBoxSize, localBoxSize }));
    pGameObject->getTransform().position = position;
    //pBody->setVelocity({ 0.0f,0.0f,0.0f });
    pBody->setGravity(true);

    SphereShape* pSphereShape = new SphereShape();
    pSphereShape->setRadius(1.0f);
    pBody->setShape(pSphereShape);


    GameObject* pBigSphere = EngineFunctions::InstantiateGameObject<GameObject>();
    EngineFunctions::AttachPhysicsBody(pBigSphere);
    float bigBoxSize = 10.0f;
    pBigSphere->getPhysicsBody()->setLocalBox(AxisAlignedBox({ 0.0f,0.0f,0.0f }, { bigBoxSize, bigBoxSize, bigBoxSize }));
    pBigSphere->getTransform().position = (Vector3(0, -10.0f, 0));

    SphereShape* pBigSphereShape = new SphereShape();
    pBigSphereShape->setRadius(5.0f);
    pBigSphere->getPhysicsBody()->setShape(pBigSphereShape);
    pBigSphere->getPhysicsBody()->setMass(10.0f);


    RenderMeshNode* pBigSphereRenderNode = new RenderMeshNode();
    pBigSphereRenderNode->setMesh(AssetManager<Mesh>::retrieve("sphere_mesh"));

    EngineFunctions::AssignRenderNode(pBigSphere, pBigSphereRenderNode);
    pBigSphere->getTransform().scale = Vector3{ 5.0f,5.0f,5.0f };
    pBigSphere->getPhysicsBody()->setImmovable(true);
}

void PhysicsControlPanel::caseBoxOnWeirdBox(bool bWeird, bool b45DegreePlane)
{
    EngineFunctions::physics().setActive(false);
    EngineFunctions::scene().clearScene();

    EngineFunctions::InstantiateGameObject<CameraController>();

    {
        Vector3 position{ 3.0f,10.0f,0.0f };
        Vector3 boxSize{ 2.0f, 2.0f,2.0f };
        float localBoxFudgeFactor = 1.414f;
        float mass = 1.0f;
        bool bGravity = true;
        bool b45Degree = bWeird;
        Quaternion degree45Quat; //hard coded 45,45,45 euler
        degree45Quat.x = 0.4619398f;
        degree45Quat.y = 0.1913417f;
        degree45Quat.z = 0.4619398f;
        degree45Quat.w = 0.7325378f;

        GameObject* pGameObject = EngineFunctions::InstantiateGameObject<GameObject>();

        RenderMeshNode* pRenderNode = new RenderMeshNode();
        pRenderNode->setMesh(AssetManager<Mesh>::retrieve("cube_mesh"));

        EngineFunctions::AssignRenderNode(pGameObject, pRenderNode);
        EngineFunctions::AttachPhysicsBody(pGameObject);

        pGameObject->getPhysicsBody()->setLocalBox(AxisAlignedBox({ 0,0,0 }, boxSize * 2.0f * localBoxFudgeFactor));
        pGameObject->getTransform().position = position;
        if (b45Degree) pGameObject->getTransform().rotation = degree45Quat;
        pGameObject->getTransform().scale = boxSize;

        BoxShape* pShape = new BoxShape();
        pShape->getHalfSize() = boxSize;
        pGameObject->getPhysicsBody()->setShape(pShape);
        pGameObject->getPhysicsBody()->setGravity(bGravity);
        pGameObject->getPhysicsBody()->setMass(mass);
    }

    {
        Vector3 position{ 0.0f,-1.0f,0.0f };
        Vector3 boxSize{ 10.0f, 1.0f,10.0f };
        float localBoxFudgeFactor = 1.414f;
        float mass = 15.0f;
        bool bGravity = false;
        bool b45Degree = false;
        Quaternion degree45Quat; //hard coded 45,45,45 euler
        degree45Quat.x = 0.4619398f;
        degree45Quat.y = 0.1913417f;
        degree45Quat.z = 0.4619398f;
        degree45Quat.w = 0.7325378f;

        GameObject* pGameObject = EngineFunctions::InstantiateGameObject<GameObject>();

        RenderMeshNode* pRenderNode = new RenderMeshNode();
        pRenderNode->setMesh(AssetManager<Mesh>::retrieve("cube_mesh"));

        EngineFunctions::AssignRenderNode(pGameObject, pRenderNode);
        EngineFunctions::AttachPhysicsBody(pGameObject);

        pGameObject->getPhysicsBody()->setLocalBox(AxisAlignedBox({ 0,0,0 }, boxSize * 2.0f * localBoxFudgeFactor));
        pGameObject->getTransform().position = position;
        if (b45Degree) pGameObject->getTransform().rotation = degree45Quat;
        if (b45DegreePlane) pGameObject->getTransform().rotation = Quaternion(0.3827, 0, 0, 0.9239);
        pGameObject->getTransform().scale = boxSize;

        BoxShape* pShape = new BoxShape();
        pShape->getHalfSize() = boxSize;
        pGameObject->getPhysicsBody()->setShape(pShape);
        pGameObject->getPhysicsBody()->setGravity(bGravity);
        pGameObject->getPhysicsBody()->setMass(mass);
        pGameObject->getPhysicsBody()->setImmovable(true);
    }

}

void PhysicsControlPanel::caseSphereExplosion()
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
