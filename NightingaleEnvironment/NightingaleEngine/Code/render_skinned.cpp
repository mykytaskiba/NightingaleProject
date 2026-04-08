#include "pch.h"
#include "render_skinned.h"
#include "mesh.h"
#include "skeleton.h"
#include "asset_manager.h"
#include "game_object.h"
#include "animation.h"
#include "inverse_kinematics_evaluator.h"
#include "game_object.h"
#include "ngrender.h"

void RenderSkinnedMeshNode::init(GameObject* pGameObject)
{
    m_pGameObject = pGameObject;
    assert(m_pGameObject != nullptr);

    m_pUnlitShader = AssetManager<RenderShader>::retrieve("unlit_shader");
    assert(m_pUnlitShader != nullptr);

    m_pAnimShader = AssetManager<RenderShader>::retrieve("anim_shader");
    assert(m_pAnimShader != nullptr);
    
    m_pBoneMesh = AssetManager<Mesh>::retrieve("bone_mesh");
    assert(m_pBoneMesh != nullptr);
}

void RenderSkinnedMeshNode::render(GraphicsContext& context)
{
    //shader
    assert(m_pSkeleton != nullptr);


    m_pSkeleton->setTransform(m_pGameObject->getTransform().getMatrix());

    m_evaluator.evaluate(m_animationT);
    m_pSkeleton->calcCurrentTransform();
    


    if (m_pIKevaluator != nullptr && m_pIKevaluator->m_bEnabled) {
        m_pIKevaluator->evaluate();
    }
    if (m_pAltIKEvaluator != nullptr && m_pAltIKEvaluator->m_bEnabled) {
        m_pAltIKEvaluator->evaluate();
    }

    m_pSkeleton->calcBoneMatricies();


    //shader.transferByName("uModelMatrix", transform);


    if (m_bDrawMesh) {
        context.setCurrentShader(m_pAnimShader);
        m_pAnimShader->transferByName("uColor", m_color);
        m_pSkeleton->Draw(m_pAnimShader);
    }

    if (m_bDrawDebug) {

        glDisable(GL_DEPTH_TEST); //TO DO: MOVE THIS CALL OUT OF HERE
        context.setCurrentShader(m_pUnlitShader);
        m_pUnlitShader->transferByName("uColor", Color(0.9f, 0.0f, 0.0f, 1.0f));
        m_pSkeleton->DebugDraw(context, m_pUnlitShader, m_pBoneMesh);
        glEnable(GL_DEPTH_TEST); //TO DO: MOVE THIS CALL OUT OF HERE
    }

    glDisable(GL_DEPTH_TEST); //TO DO: MOVE THIS CALL OUT OF HERE

    if (m_pIKevaluator != nullptr && m_pIKevaluator->m_bEnabled) {
        m_pIKevaluator->draw(context);
    }
    if (m_pAltIKEvaluator != nullptr && m_pAltIKEvaluator->m_bEnabled) {
        m_pAltIKEvaluator->draw(context);
    }
    glEnable(GL_DEPTH_TEST); //TO DO: MOVE THIS CALL OUT OF HERE
}

void RenderSkinnedMeshNode::setSkeleton(Skeleton* pSkeleton)
{
    m_pSkeleton = pSkeleton;
}

void RenderSkinnedMeshNode::setColor(Color const& color)
{
    m_color = color;
}

void RenderSkinnedMeshNode::setDrawMode(bool drawMesh, bool drawDebug)
{
    m_bDrawMesh = drawMesh;
    m_bDrawDebug = drawDebug;
}

void RenderSkinnedMeshNode::bindPose()
{
    m_pSkeleton->bindPose();
    m_evaluator.unbind();
}

void RenderSkinnedMeshNode::setAnimation(Animation* pAnimation)
{
    m_pAnimation = pAnimation;
    m_pSkeleton->bindPose();
    m_pSkeleton->calcCurrentTransform();
    m_evaluator.bindEvaluator(m_pAnimation, m_pSkeleton);
}

void RenderSkinnedMeshNode::setT(float t)
{
    m_animationT = t;
}
