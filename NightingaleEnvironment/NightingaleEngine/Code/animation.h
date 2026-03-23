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

    vector<float> timeValues{};
    vector<Channel> channels{};

    vector<TPositionChannel> positionChannels;
    vector<TRotationChannel> rotationChannels;
    vector<TScalingChannel> scalingChannels;
};

struct Channel {
public:
    string targetNodeName;
    vector<Transform> values;
};

template <typename T>
struct AnimationChannel {
    friend class AnimationBuilder;
    friend class Skeleton; //TO DO: REMOVE THIS FRIEND
public:
    string targetNodeName;
    vector<float> timeValues;
    vector<T> values;

    void addValue(float t, T const& value) {
        timeValues.push_back(t);
        values.push_back(value);
    }
    void finalize() {
        timeValues.shrink_to_fit();
        values.shrink_to_fit();
    }
       
};