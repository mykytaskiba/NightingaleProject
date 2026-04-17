#pragma once
#include "shape.h"
#include "property_visitor.h"

class SphereShape : public Shape {
public:
    SphereShape() : Shape(ShapeType::Sphere) {}

    void setRadius(float radius) { m_radius = radius; }
    float getRadius() { return m_radius; }

    void properties(IPropertyVisitor& visitor) override {
        visitor("radius", m_radius);
    }

    void calculateMomentOfInertia(Matrix3x3& momentInertiaInverse, Quaternion const& rotation, float mass) override;
private:
    float m_radius{ 0.0f };

};