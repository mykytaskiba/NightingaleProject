#pragma once
#include "ngmath.h"
#include "transform.h"

template <typename T> 
class AnimationChannel;

struct Channel;

class Animation {
    friend class AnimationBuilder;
    friend class Skeleton; //TO DO: REMOVE THIS FRIEND
public:
    float duration;
    float ticksPerSecond;

    using TPositionChannel = AnimationChannel<Vector3>;
    using TRotationChannel = AnimationChannel<Quaternion>;
    using TScalingChannel = AnimationChannel<float>;

    std::vector<float> timeValues{};
    std::vector<Channel> channels{};

    std::vector<TPositionChannel> positionChannels;
    std::vector<TRotationChannel> rotationChannels;
    std::vector<TScalingChannel> scalingChannels;
};

struct Channel {
public:
    string targetNodeName;
    std::vector<Transform> values;
};

template <typename T>
struct AnimationChannel {
    friend class AnimationBuilder;
    friend class Skeleton; //TO DO: REMOVE THIS FRIEND
public:
    string targetNodeName;
    std::vector<float> timeValues;
    std::vector<T> values;

    void addValue(float t, T const& value) {
        timeValues.push_back(t);
        values.push_back(value);
    }
    void finalize() {
        timeValues.shrink_to_fit();
        values.shrink_to_fit();
    }
       
};