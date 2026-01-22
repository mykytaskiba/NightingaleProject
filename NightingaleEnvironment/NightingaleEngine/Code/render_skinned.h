#pragma once
#include "render_node.h"
#include "ngmath.h"
#include "animation_evaluator.h"

class Mesh;
class Skeleton;
class RenderShader;
class Animation;
class InverseKinematicEvaluator;

class RenderSkinnedMeshNode : public RenderNode {
public:
    friend class ProjectThree; //TO DO: remove this

    virtual void init(GameObject* pGameObject);
    virtual void render(GraphicsContext&);

    void setSkeleton(Skeleton* pMesh);
    void setColor(Color const& color);

    void setDrawMode(bool drawMesh, bool drawDebug);

    void bindPose();
    void setAnimation(Animation*);
    void setT(float t); 
private:
    Skeleton* m_pSkeleton;
    Mesh* m_pBoneMesh;
    GameObject* m_pGameObject;
    Color m_color{ 1.0f,1.0f,1.0f,1.0f };

    Animation* m_pAnimation;

    AnimationEvaluator m_evaluator;
    float m_animationT = 0.0f;

    InverseKinematicEvaluator* m_pIKevaluator;
    InverseKinematicEvaluator* m_pAltIKEvaluator;

    bool m_bDrawMesh{ true };
    //TO DO: probably move this out eventually
    bool m_bDrawDebug{ false };

    RenderShader* m_pUnlitShader;
    RenderShader* m_pAnimShader;
};