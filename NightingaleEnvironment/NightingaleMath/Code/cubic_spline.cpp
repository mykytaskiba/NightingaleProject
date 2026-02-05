#include "defines.h"
#include "cubic_spline.h"
#include <cmath>

void CubicSpline::addValue(float value)
{
    m_values.push_back(value);
}

void CubicSpline::finalize()
{
    m_derivatives.clear();
    //TO DO: Automatically recalculate derivatives when adding values
    m_derivatives.push_back(0.0f); //first derivative
    for (size_t i{ 1u }; i < m_values.size() - 1u; ++i) {
        float derivative = (m_values[i + 1u] - m_values[i - 1u]) / 2.0f;
        m_derivatives.push_back(derivative);
    }
    m_derivatives.push_back(0.0f); //final derivative
}

void CubicSpline::solveHelper(float& a, float& b, float& c, float& d) const
{
    //This is equivalent to solving an inverse of the interpolation matrix for a spline curve
    //going through t = 0 and t = 1, with derivatives at t = 0 and t = 1

    std::swap(b, c);
    float tempC = c;
    c = -3.0f * a + 3.0f * c - 2.0f * b - 1.0f * d;
    d = 2.0f * a - 2.0f * tempC + b + d;
}

float CubicSpline::evaluate(float t) const
{


    size_t valueCount = m_values.size();
    if (valueCount == 0u) {
        //assert(0);
        return 0.0f;
    }

    if (t < 0.0f || t > 1.0f) {
        //assert(0);
        return 0.0f;
    }

    if (t == 1.0f) t -= 0.00001;

    uint intervalCount = valueCount - 1u;
    double globalT = (t * (double)intervalCount);
    size_t valueIndex = (size_t)globalT;


    float temp;
    float relativeT = std::modf(globalT, &temp);

    Vector4 values = Vector4(m_values[valueIndex], m_values[valueIndex + 1u],
        m_derivatives[valueIndex], m_derivatives[valueIndex + 1u]);

    //solve 4x4 inverse matrix (but a trick used here)
    solveHelper(values[0], values[1], values[2], values[3]);
    Vector4 const& functionValues = values;

    float result = functionValues[0] +
        relativeT * (functionValues[1] +
            relativeT * (functionValues[2] +
                relativeT * functionValues[3]));

    return result;


}

void CubicSpline3D::addValue(Vector3 value)
{
    m_xSpline.addValue(value[0]);
    m_ySpline.addValue(value[1]);
    m_zSpline.addValue(value[2]);
}

Vector3 CubicSpline3D::evaluate(float t) const
{
    Vector3 result;
    result[0] = m_xSpline.evaluate(t);
    result[1] = m_ySpline.evaluate(t);
    result[2] = m_zSpline.evaluate(t);
    return result;
}

void CubicSpline3D::finalize()
{
    m_xSpline.finalize();
    m_ySpline.finalize();
    m_zSpline.finalize();
}

void SplineRepresentation::represent(CubicSpline3D const& curve, float deltaStep)
{
    m_representationValues.clear();

    uint totalSteps = ((uint)(1.0f / deltaStep) - 1u);
    m_representationValues.reserve((size_t)(totalSteps + 1u));

    for (uint i{ 0u }; i < totalSteps; ++i) {
        float t = deltaStep * (float)i;

        m_representationValues.push_back(curve.evaluate(t));
    }

    m_representationValues.push_back(curve.evaluate(1.0f));

}

/*
void SplineRepresentation::draw(GraphicsContext& context)
{
    for (size_t i{ 0u }; i < m_representationValues.size() - 1u; ++i) {
        context.drawLine(m_representationValues[i], m_representationValues[i + 1u]);
    }
}*/
