#include "arc_length_table.h"
#include "cubic_spline.h"
#include "vector.h"

void ArcLengthTable::evaluate(CubicSpline3D const& spline)
{
    float const maxDeltaT = 0.05f; //force at least 20 steps of 0.05
    float const maxArcDiscreptancy = 0.02f;

    float currT = 0.0f;
    float currLength = 0.0f;

    m_tValues.push_back(currT);
    m_length.push_back(currLength);
    float currStep = maxDeltaT;

    while (true) {
        float nextT = currT + currStep; //take a step
        Vector3 currVal = spline.evaluate(currT);

        if (nextT >= 1.0f) { //if we are finished
            nextT = 1.0f;
        }

        Vector3 halfVal = spline.evaluate((currT + nextT) / 2.0f);
        Vector3 nextVal = spline.evaluate(nextT);

        //more precise distance delta
        float preciseDistDelta = (currVal - halfVal).magnitude() + (halfVal - nextVal).magnitude();
        float distDelta = (currVal - nextVal).magnitude();
        if (preciseDistDelta - distDelta > maxArcDiscreptancy) {
            //we need more precision
            currStep = currStep / 2.0f;
            continue;
        }
        currLength += distDelta;
        //meets delta T and arc discreptancy expectations
        m_tValues.push_back(nextT);
        m_length.push_back(currLength);
        currT = nextT;

        if (currT >= 1.0f) {
            m_totalLength = currLength;
            break;
        }

    }

}

float ArcLengthTable::getTFromLength(float length) const
{
    uint lower = 0;
    uint upper = m_length.size() - 1u;
    uint mid = 0;
    while (upper >= lower) {
        mid = ((lower + upper) / 2u);
        if (mid == m_length.size() - 1u) return 1.0f;

        float minLength = m_length[mid];
        float maxLength = m_length[mid + 1u];

        if (length >= minLength && length < maxLength)
        {
            //found a mid
            float tInterpolant = (length - minLength) / (maxLength - minLength);
            return m_tValues[mid] * (1.0f - tInterpolant) + m_tValues[mid + 1u] * (tInterpolant);
        }
        if (length < minLength) {
            upper = mid;
        }
        if (length >= maxLength) {
            lower = mid + 1u;
        }
    }

    return 0.0f;
}

float ArcLengthTable::getTotalLength() const
{
    return m_totalLength;
}

