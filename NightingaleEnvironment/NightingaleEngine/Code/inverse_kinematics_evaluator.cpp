#include "pch.h"
#include "inverse_kinematics_evaluator.h"
#include "graphics_context.h"
#include "asset_manager.h"
#include "ngmath.h"

void InverseKinematicEvaluator::bind(Skeleton* pSkeleton, string const& bindNode, uint bindDepth)
{
    //clear previous data
    m_pSkeleton = nullptr;
    m_limbs.clear();
    m_totalLength = 0.0f;

    assert(pSkeleton != nullptr);
    assert(bindDepth > 0);
    
    m_pSkeleton = pSkeleton;
    m_pSkeleton->bindPose();
    m_pSkeleton->calcCurrentTransform();

    auto it = m_pSkeleton->m_nodeNameMap.find(bindNode);
    assert(it != m_pSkeleton->m_nodeNameMap.end());
    uint nodeIndex = (*it).second;

    //m_IKchain.reserve(bindDepth);
    m_limbs.reserve(bindDepth);

    uint depth = 0;
    while (nodeIndex != 0 && depth < bindDepth) {

        uint parentIndex = m_pSkeleton->m_nodes[nodeIndex].parent;
        if (parentIndex == 0) {
            break;
        }

        float dist = (pSkeleton->nodePosition(nodeIndex)- pSkeleton->nodePosition(parentIndex)).magnitude();

        if (dist != 0.0f) {
            ++depth;
            m_totalLength += dist;

            Limb limb;
            limb.fromIdx = parentIndex;
            limb.toIdx = nodeIndex;
            limb.length = dist;
            m_limbs.push_back(limb);
        }


        nodeIndex = parentIndex;
    }


}

void InverseKinematicEvaluator::evaluate()
{

    

    prepare();

    if (!m_bIKOutsideOfDistance) {
        float distToTarget = (m_absoluteTarget - root()).magnitude();
        if (distToTarget > m_totalLength) {
            return;
        }
    }

    bool bFabrikRunning{ true };
    uint iterationCount{ 0u };
    Vector3 prevEndEffectorPosition = endEffector();
    while (bFabrikRunning) {
        fabricForward();
        fabricBackward();

        ++iterationCount;
        if (iterationCount >= m_maxFabricIteration) bFabrikRunning = false;

        Vector3 currentEndEffector = endEffector();
        if ((currentEndEffector - prevEndEffectorPosition).magnitude() < m_endEffectorDeltaTolerance) {
            bFabrikRunning = false;
        }
    }
    setSkeletonOrientations();

    /* set orientation
    float distToTarget = (m_IKTarget - root()).magnitude();
    if (distToTarget > m_totalLength) {

        
        for (auto it_node{ m_IKchain.begin() }; it_node != m_IKchain.end()-1u; ++it_node) {
            uint nodeIndex = (*it_node);
            uint prevNode = *(it_node + 1u);

            Vector3 currentVec = (m_pSkeleton->nodePosition(prevNode) - m_pSkeleton->nodePosition(nodeIndex));
            currentVec.normalize_equal();
            Vector3 toTarget = (m_IKTarget - m_pSkeleton->nodePosition(nodeIndex));
            toTarget.normalize_equal();

            m_debug1 = m_pSkeleton->nodePosition(nodeIndex);
            m_debug2 = m_debug1 + toTarget;

            m_debugCurrent = m_debug1 + currentVec;



            float diff = toTarget.dot(currentVec);

            Vector3 axis = currentVec.cross(toTarget).normalized();
            m_debugCross = m_debug1 + axis;
            float angle = acos(currentVec.dot(toTarget));
            //Vector3 bindNodeUp = m_pSkeleton->bindPoseNodeOrientation(nodeIndex).normalized();
            //Vector3 bindNodeUp = m_pSkeleton->currentNodeUp(nodeIndex).normalized();

            Quaternion rotation(axis, angle);

            Matrix4x4 translation = Matrix4x4::CreateTranslation(1.0f * m_pSkeleton->nodePosition(nodeIndex));
            Matrix4x4 translationInv = Matrix4x4::CreateTranslation(-1.0f*m_pSkeleton->nodePosition(nodeIndex));
            //m_pSkeleton->m_nodes[nodeIndex].transform = m_pSkeleton->m_nodes[nodeIndex].transform * rotation.getRotation();
            m_pSkeleton->m_nodeTransforms[nodeIndex] = translation * rotation.getRotation() * translationInv * m_pSkeleton->m_nodeTransforms[nodeIndex];

            m_pSkeleton->recalcCurrentTransformPartial(nodeIndex);
        }

        return;
    }*/
}

void InverseKinematicEvaluator::draw(GraphicsContext& context)
{

    if (m_bDrawIKBones) {
        for (auto it{ m_limbs.begin() }; it != m_limbs.end(); ++it) {

            context.drawLine((*it).from, (*it).to);
            context.drawSphere((*it).from, 0.05f);
        }

        context.drawSphere(m_reverseTarget, 0.1f, Color(1.0f, 0.1f, 0.1f, 1.0f));
    }

    if (m_bDrawEndEffector) {
        context.drawSphere(m_IKTarget, 0.1f, Color(0.1f, 1.0f, 0.1f, 1.0f));
    }
}

void InverseKinematicEvaluator::setSkeletonOrientations() {

    for (auto it{ m_limbs.rbegin() }; it != m_limbs.rend(); ++it) {

        Vector3 currentVec = (m_pSkeleton->nodePosition(it->toIdx) - m_pSkeleton->nodePosition(it->fromIdx));
        currentVec.normalize_equal();
        Vector3 toTarget = (it->to - m_pSkeleton->nodePosition(it->fromIdx));
        toTarget.normalize_equal();

        Vector3 axis = currentVec.cross(toTarget).normalized();
        float angle = acos(currentVec.dot(toTarget));

        Quaternion rotation(axis, angle);

        Matrix4x4 translation;
        translation.make_translation(1.0f * m_pSkeleton->nodePosition(it->fromIdx));
        Matrix4x4 translationInv;
        translationInv.make_translation(-1.0f * m_pSkeleton->nodePosition(it->fromIdx));

        m_pSkeleton->m_nodeTransforms[it->fromIdx] = translation * rotation.getRotation() * translationInv * m_pSkeleton->m_nodeTransforms[it->fromIdx];
        m_pSkeleton->recalcCurrentTransformPartial(it->fromIdx);

    }


}


Vector3 const& InverseKinematicEvaluator::endEffector() const
{
    return (*m_limbs.begin()).to;
}

Vector3 const& InverseKinematicEvaluator::root() const
{
    return (*m_limbs.rbegin()).from;
}

void InverseKinematicEvaluator::fabricForward()
{

    Vector3 target = m_IKTarget;
    for (auto it{ m_limbs.begin() }; it != m_limbs.end(); ++it) {

        Vector3 translation = target - (*it).from;
        (*it).to = target;

        (*it).from = (*it).to - (*it).length * translation.normalized();
        
        target = (*it).from;
    }
}

void InverseKinematicEvaluator::fabricBackward()
{
    Vector3 target = m_reverseTarget;
    for (auto it{ m_limbs.rbegin() }; it != m_limbs.rend(); ++it) {

        Vector3 translation = target - (*it).to;
        (*it).from = target;

        (*it).to = (*it).from - (*it).length * translation.normalized();

        target = (*it).to;
    }
}


void InverseKinematicEvaluator::prepare()
{
    for (auto it{ m_limbs.begin() }; it != m_limbs.end(); ++it) {
        (*it).to = m_pSkeleton->nodePosition((*it).toIdx);
        (*it).from = m_pSkeleton->nodePosition((*it).fromIdx);
    }
    m_reverseTarget = (*(--m_limbs.end())).from;

}

