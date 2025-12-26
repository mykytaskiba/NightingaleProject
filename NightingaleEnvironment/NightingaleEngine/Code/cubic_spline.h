#pragma once
#include "defines.h"
#include "vector.h"
#include "matrix.h"


class GraphicsContext;

class CubicSpline {

private:
    vector<float> m_values{};
    vector<float> m_derivatives{};
public:
    void addValue(float value);
    void finalize();
    
    float evaluate(float t) const;

private:
    void solveHelper(float& val1, float& val2, float& dv1, float& dv2) const;


};

class CubicSpline3D {
private:

    CubicSpline m_xSpline;
    CubicSpline m_ySpline;
    CubicSpline m_zSpline;

public:
    void addValue(Vector3 value);
    Vector3 evaluate(float t) const;
    void finalize();
};

class SplineRepresentation {

private:
    vector<Vector3> m_representationValues;
public:
    void represent(CubicSpline3D const& curve, float deltaStep);
    void draw(GraphicsContext& context);
};