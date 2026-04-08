#include "pch.h"
#include "project_three_coobject.h"
#include "engine_internals.h"
#include "asset_manager.h"
#include "mesh.h"
#include "skeleton.h"
#include "project_one_gameobject.h"
#include "render_skinned.h"
#include "engine_functions.h"

/*
void ProjectThreeRenderHook::render(GraphicsContext& context)
{
    if (m_projectThree.m_bSmoothMoveTarget && m_projectThree.m_IKevaluator.m_bDrawEndEffector) {
        Transform transform;
        transform.position = m_projectThree.m_realTarget;

        assert(false);
        //-->transform.scale = 0.09f;

        context.setCurrentShader(m_pUnlitShader);
        m_pUnlitShader->transferByName("uColor", Color(0.1f, 0.1f, 0.9f, 1.0f));
        m_pUnlitShader->transferByName("uModelMatrix", transform.getMatrix());
        m_pSphere->Draw();

    }
}

void ProjectThreeRenderHook::initRaw()
{
    m_pSphere = AssetManager<Mesh>::retrieve("sphere_mesh");
    assert(m_pSphere != nullptr);

    m_pUnlitShader = AssetManager<RenderShader>::retrieve("unlit_shader");
    assert(m_pUnlitShader);
}

ProjectThree::ProjectThree(ProjectOneGameObject& projectOne) : m_projectOne(projectOne), m_renderHook(*this)
{
}

void ProjectThree::init()
{
    EngineInternals::Renderer().registerRenderable(&m_renderHook); // hack for rendering with a specific render hook
    m_renderHook.initRaw();

    m_pRenderNode = dynamic_cast<RenderSkinnedMeshNode*>(m_projectOne.m_pRenderNode);
    assert(m_pRenderNode != nullptr);
    
    m_pSkeleton = m_pRenderNode->m_pSkeleton;
    
    m_IKevaluator.bind(m_pSkeleton, "mixamorig:LeftHand", m_limbCount);
    m_altIKEvaluator.bind(m_pSkeleton, "mixamorig:RightHand", m_limbCount);
    m_lastLimbCount = m_limbCount;

    m_pRenderNode->m_pIKevaluator = &m_IKevaluator;
    m_pRenderNode->m_pAltIKEvaluator = &m_altIKEvaluator;

}


void ProjectThree::tick()
{
    //ImGui::BeginChild("ProjectThreePanel");
    ImGui::NewLine();
    ImGui::Checkbox("Project Three", &m_uiActive);
    if (m_uiActive) {

        ImGui::Checkbox("Enable IK", &m_IKevaluator.m_bEnabled);
        ImGui::Checkbox("IK Outside Of Range", &m_IKevaluator.m_bIKOutsideOfDistance);
        ImGui::Checkbox("Render End Effector", &m_IKevaluator.m_bDrawEndEffector);
        ImGui::Checkbox("Render IK Limbs", &m_IKevaluator.m_bDrawIKBones);
        ImGui::Checkbox("Enable IK Smooth Move", &m_bSmoothMoveTarget);
        ImGui::SliderFloat("Smooth Move Speed", &m_smoothMoveSpeed, 0.2f, 10.0f);


        ImGui::SliderFloat3("IK Target", &m_realTarget[0], -10.0f, 10.0f, "%.1f");

        if (ImGui::Button("Move Closer Using Project2")) {

            float distOffset = m_IKevaluator.m_totalLength / 1.2f;
            Vector3 toTarget = (m_realTarget - m_projectOne.getTransform().position).normalized();

            Vector3 finish = m_realTarget - toTarget * distOffset;

            m_projectOne.m_ProjectTwo.setPathToPoint(finish);
            m_projectOne.m_ProjectTwo.activate();

            m_projectOne.m_bEnableWASDMovement = false;
            m_IKevaluator.m_bEnabled = true;
            m_IKevaluator.m_bIKOutsideOfDistance = false;
        }

        if (ImGui::SliderInt("Limb Count", &m_limbCount, 1, 6));
    }
    //ImGui::EndChild();

    if (m_lastLimbCount != m_limbCount) {
        m_lastLimbCount = m_limbCount;
        m_IKevaluator.bind(m_pSkeleton, "mixamorig:LeftHand", m_limbCount);
        m_altIKEvaluator.bind(m_pSkeleton, "mixamorig:RightHand", m_limbCount);
    }

    if (m_bProjectFourOverride) return;

    if (m_bSmoothMoveTarget) {

        m_IKTarget = m_IKevaluator.endEffector();

        Vector3 toTarget = m_realTarget - m_IKTarget;
        float maxMoveThisFrame = m_smoothMoveSpeed * EngineFunctions::delta_time();
        if (toTarget.magnitude() > maxMoveThisFrame) {
            toTarget.normalize_equal() *= maxMoveThisFrame;
        }
        m_IKTarget += toTarget;
    }
    else {
        m_IKTarget = m_realTarget;
    }

    
    m_IKevaluator.m_IKTarget = m_IKTarget;
    m_IKevaluator.m_absoluteTarget = m_realTarget;


}

void ProjectThree::shutdown()
{
}

void ProjectThree::OverridePosition(Vector3 left, Vector3 right)
{
    m_bProjectFourOverride = true;

    m_IKevaluator.m_IKTarget = left;
    m_IKevaluator.m_absoluteTarget = left;
    m_altIKEvaluator.m_IKTarget = right;
    m_altIKEvaluator.m_absoluteTarget = right;

    m_IKevaluator.m_bEnabled = true;
    m_altIKEvaluator.m_bEnabled = true;

    //m_IKevaluator.m_IKTarget = m_IKTarget;
    //m_IKevaluator.m_absoluteTarget = m_realTarget;
}

void ProjectThree::DisableOverride()
{
    m_IKevaluator.m_bEnabled = false;
    m_altIKEvaluator.m_bEnabled = false;
    m_bProjectFourOverride = false;
}

*/