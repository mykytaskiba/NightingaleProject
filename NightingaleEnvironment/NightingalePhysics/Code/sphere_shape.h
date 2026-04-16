#pragma once
#include "shape.h"

class SphereShape : public Shape {
public:
    SphereShape() : Shape(ShapeType::Sphere) {}

    void setRadius(float radius) { m_radius = radius; }
    float getRadius() { return m_radius; }

private:
    float m_radius{ 0.0f };

};