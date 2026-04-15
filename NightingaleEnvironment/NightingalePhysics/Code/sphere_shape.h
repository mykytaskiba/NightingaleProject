#pragma once
#include "shape.h"

class SphereShape : public Shape {
public:
    SphereShape() : Shape(ShapeType::Sphere) {}

    float m_radius{ 0.0f };

};