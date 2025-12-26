#include "pch.h"
#include "animation_builder.h"
#include "animation.h"
#include "loader.h"
#include "transform.h"

AnimationBuilder::AnimationBuilder(aiScene const* pScene) {

    assert(pScene->mNumAnimations == 1);

    aiAnimation const* pAnim = pScene->mAnimations[0];

    m_pAnimation = new Animation();
    m_pAnimation->duration = pAnim->mDuration;
    m_pAnimation->ticksPerSecond = pAnim->mTicksPerSecond;

    uint numChannel = pAnim->mNumChannels;

    loadTimeKeys(pAnim->mChannels[0]);

    for (uint i = 0; i < pAnim->mNumChannels; ++i) {
        loadChannel(pAnim->mChannels[i], i);
    }

    m_pAnimation->timeValues.shrink_to_fit();
    m_pAnimation->channels.shrink_to_fit();
    assert(m_pAnimation->channels.size() == numChannel);

    m_bSuccess = true;
}

bool AnimationBuilder::isSuccessful() const
{
    return m_bSuccess;
}

Animation* AnimationBuilder::getAnimation() const
{
    return m_pAnimation;
}

void AnimationBuilder::loadTimeKeys(aiNodeAnim const* animChannel)
{
    //NOTE: Assumption is all channels on the animation are either 1 key or all share the same number of keys
    //This code finds a shared time values for all the channels in the animation
    
    bool bEmpty = false;

    bEmpty = m_pAnimation->timeValues.size() == 0;
    if (bEmpty && animChannel->mNumRotationKeys > 1) {
        for (uint i = 0; i < animChannel->mNumRotationKeys; ++i) {
            aiQuatKey const& key = animChannel->mRotationKeys[i];
            m_pAnimation->timeValues.push_back(key.mTime);
        }
    }

    bEmpty = m_pAnimation->timeValues.size() == 0;
    if (bEmpty && animChannel->mNumPositionKeys > 1) {
        for (uint i = 0; i < animChannel->mNumPositionKeys; ++i) {
            aiVectorKey const& key = animChannel->mPositionKeys[i];
            m_pAnimation->timeValues.push_back(key.mTime);
        }
    }


    bEmpty = m_pAnimation->timeValues.size() == 0;
    if (bEmpty && animChannel->mNumScalingKeys > 1) {
        for (uint i = 0; i < animChannel->mNumScalingKeys; ++i) {
            aiVectorKey const& key = animChannel->mScalingKeys[i];
            m_pAnimation->timeValues.push_back(key.mTime);
        }
    }


    m_numKeys = m_pAnimation->timeValues.size();
    assert(m_numKeys > 1);

}

void AnimationBuilder::loadChannel(aiNodeAnim const* animChannel, uint channelId)
{
    
    
    m_pAnimation->channels.push_back(Channel());
    Channel& channel = (* m_pAnimation->channels.rbegin());

    string nodeName = animChannel->mNodeName.C_Str();
    channel.targetNodeName = nodeName;

    Transform superTransform; //super transform if no other transform is picked up

    {//super transform creation code
        aiVectorKey const& posKey = animChannel->mPositionKeys[0];
        aiQuatKey const& rotKey = animChannel->mRotationKeys[0];
        aiVectorKey const& sclKey = animChannel->mScalingKeys[0];
        
        superTransform.position = Loader::fromAIVector3(posKey.mValue);
        superTransform.rotation = Loader::fromAIQuat(rotKey.mValue);
        superTransform.scale = sclKey.mValue.x;
    }

    channel.values.resize(m_numKeys, superTransform);
    
    for (uint i = 1; i < m_numKeys; ++i) {

        Transform& keyTransform = channel.values[i];

        if (animChannel->mNumPositionKeys == m_numKeys) {
            aiVectorKey const& key = animChannel->mPositionKeys[i];
            keyTransform.position = Loader::fromAIVector3(key.mValue);
        }

        if (animChannel->mNumScalingKeys == m_numKeys) {
            aiVectorKey const& key = animChannel->mScalingKeys[i];
            keyTransform.scale = key.mValue.x;
        }

        if (animChannel->mNumRotationKeys == m_numKeys) {
            aiQuatKey const& key = animChannel->mRotationKeys[i];
            keyTransform.rotation = Loader::fromAIQuat(key.mValue);
        }

    }
}
