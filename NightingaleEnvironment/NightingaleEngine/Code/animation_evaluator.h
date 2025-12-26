#pragma once
#include "defines.h"

class Animation;
class Skeleton;

//Evaluates the state of the animation (interpolation, etc)
//keeps the state to reuse between frames
class AnimationEvaluator {

private:
    bool m_bHasConnection{ false };

    Animation* m_pAnimation{ nullptr };
    Skeleton* m_pSkeleton{ nullptr };
    vector<uint> m_channelToNodeMap{};
public:
    void bindEvaluator(Animation*, Skeleton*);
    void unbind();
    void evaluate(float t);

};