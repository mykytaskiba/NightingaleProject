#pragma once
#include "shape.h"
#include "property_visitor.h"

class BoxShape : public Shape {
public:
    BoxShape() : Shape(ShapeType::Box) {}


    void properties(IPropertyVisitor& visitor) override {
        visitor("half_size", m_halfSize);
    }

    void calculateMomentOfInertia(Matrix3x3& momentInertiaInverse, Quaternion const& rotation, float mass) override;

    void getAxisFromRotation(Quaternion const& rotation, Vector3& axis1, Vector3& axis2, Vector3& axis3) const;

    Vector3 const& getHalfSize() const { return m_halfSize; }
    Vector3& getHalfSize() { return m_halfSize; }

private:


    Vector3 m_halfSize{ 0.0f,0.0f,0.0f };

};