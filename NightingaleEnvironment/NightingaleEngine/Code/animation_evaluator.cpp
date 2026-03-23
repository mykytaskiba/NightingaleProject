#include "pch.h"
#include "animation_evaluator.h"
#include "animation.h"
#include "skeleton.h"
#include "ngmath.h"
#include "transform.h"

using TPositionChannel = Animation::TPositionChannel;
using TRotationChannel = Animation::TRotationChannel;
using TScalingChannel = Animation::TScalingChannel;

void AnimationEvaluator::bindEvaluator(Animation* pAnimation, Skeleton* pSkeleton)
{
    unbind();
    m_bHasConnection = true;
    m_pAnimation = pAnimation;
    m_pSkeleton = pSkeleton;

    vector<Channel>& channels = m_pAnimation->channels;

    for (auto it = channels.begin(); it != channels.end(); ++it) {
        m_channelToNodeMap.push_back(pSkeleton->m_nodeNameMap[(*it).targetNodeName]);
    }

}

void AnimationEvaluator::unbind()
{
    m_bHasConnection = false;
    m_pAnimation = nullptr;
    m_pSkeleton = nullptr;
    m_channelToNodeMap.clear();
}

void AnimationEvaluator::evaluate(float t)
{
    if (!m_bHasConnection) return;

    if (t >= 1.0f) {
        t = 0.9999f;
    }

    float animT = t * m_pAnimation->duration;
    vector<float>& timeValues = m_pAnimation->timeValues;
    uint lowerKey = 0, upperKey = 0;
    for (uint i = 0; i < timeValues.size()-1; ++i) {
        if (animT >= timeValues[i] && animT < timeValues[i+1]) {
            lowerKey = i;
            upperKey = i + 1;
            break;
        }
    }

    if (lowerKey == upperKey) {//TO DO: THIS WAS A TEMPORARY FIX FOR THE THROWING ANIMATIOn
        lowerKey = timeValues.size() - 2;
        upperKey = timeValues.size()-1;
        t = 0.9999f;
        animT = t * timeValues[upperKey];
    }
    //assert(lowerKey != upperKey);

    float relativeT = (animT - timeValues[lowerKey]) / (timeValues[upperKey] - timeValues[lowerKey]);

    
    uint channelIndex = 0;
    for (auto it = m_channelToNodeMap.begin(); it != m_channelToNodeMap.end(); ++it) {
        uint nodeIndex = *it;

        Channel& channel = m_pAnimation->channels[channelIndex];
        
        Transform t = Transform::interpolate(channel.values[lowerKey],channel.values[upperKey], relativeT);

        m_pSkeleton->m_nodes[nodeIndex].transform = t.getMatrix();

        ++channelIndex;
    }

    /*
    m_pAnimation
    for (uint i = 0; i < pAnimation->positionChannels.size(); ++i) {
        auto it = m_nodeNameMap.find(pAnimation->positionChannels[i].targetNodeName);
        if (it == m_nodeNameMap.end()) {
            std::cout << pAnimation->positionChannels[i].targetNodeName << " not found!\n";
            continue;
        }
        uint nodeId = (*it).second;

        Vector3 pos = pAnimation->positionChannels[i].values[0];
        Quaternion rot = pAnimation->rotationChannels[i].values[0];
        float scale = pAnimation->scalingChannels[i].values[0];

        //Matrix4x4 nodeMat = Matrix4x4::CreateTranslation(pos) * rot.GetRotation() * Matrix4x4::CreateScale(scale);
        Matrix4x4 nodeMat = Matrix4x4::CreateTranslation(pos) * rot.GetRotation() * Matrix4x4::CreateScale(scale);
        m_nodes[nodeId].transform = nodeMat;
    }*/
}
