#pragma once
#include "defines.h"

class CubicSpline3D;

class ArcLengthTable {

private:
    std::vector<float> m_length{};
    std::vector<float> m_tValues{};

    float m_totalLength{ 0.0f };

public:

    void evaluate(CubicSpline3D const& spline);

    float getTFromLength(float length) const;
    float getTotalLength() const;

};

