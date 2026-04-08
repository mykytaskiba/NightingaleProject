#pragma once
#include "defines.h"
#include "inverse_kinematics_evaluator.h"
#include "ngmath.h"

/*
class ProjectOneGameObject;
class ProjectThree;
class Mesh;
class RenderShader;
class RenderSkinnedMeshNode;

class ProjectThreeRenderHook : public RenderNode {
public:
    ProjectThree& m_projectThree;
    ProjectThreeRenderHook(ProjectThree& prjThree) : m_projectThree(prjThree) {}

    virtual void render(GraphicsContext& context);

    void initRaw();

    Mesh* m_pSphere{ nullptr };
    RenderShader* m_pUnlitShader{ nullptr };
};

class ProjectThree {
    ProjectOneGameObject& m_projectOne;
public:
    ProjectThree(ProjectOneGameObject&);
    void init();
    void tick();
    void shutdown();

    Vector3 m_realTarget{-0.2f, -5.5f, 1.5f};
    Vector3 m_IKTarget{ -0.2f, -5.5f, 1.5f };
    InverseKinematicEvaluator m_IKevaluator{};
    InverseKinematicEvaluator m_altIKEvaluator{};

    bool m_bProjectFourOverride{ false };
    void OverridePosition(Vector3 left, Vector3 right);
    void DisableOverride();

    int m_limbCount{ 3u };
    int m_lastLimbCount{ 0u };

    bool m_bSmoothMoveTarget{ true };
    float m_smoothMoveSpeed{ 3.0f };

    bool m_uiActive{ false };
    
    RenderSkinnedMeshNode* m_pRenderNode;
    Skeleton* m_pSkeleton;

    
    ProjectThreeRenderHook m_renderHook; //TO DO: eventually remove this
};

*/