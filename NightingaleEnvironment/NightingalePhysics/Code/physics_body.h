#pragma once
#include "ngmath.h"
#include "phys_defines.h"
#include "property_provider.h"
#include "json_object.h"
#include "factory.h"
#include <memory>
#include "property_visitor.h"
#include "shape.h"

class IPropertyVisitor;

//Any physics body that needs to be simulated
//Could be rigid or soft
class PhysicsBody : public IPropertyProvider, public IJSONObject, public IFactoryElement {
private:

	bool m_bUseGravity{ false };
	
	Vector3 m_position{ 0.0f,0.0f,0.0f };
	Vector3 m_velocity{ 0.0f,0.0f,0.0f };


	AxisAlignedBox m_globalBox{};
	AxisAlignedBox m_localBox{};

	Shape* m_pShape{ nullptr };

	void calculateGlobalBox();
public:

	JSON_PARENT(PhysicsBody, 1u, "json.physicsbody")

	void properties(IPropertyVisitor& visitor) override {

		visitor.pushMeta(MetaData::ReadOnly());
		visitor("position",m_position);
		visitor.popMeta();
		visitor("velocity",m_velocity);

		visitor("shape", m_pShape);

		visitor("local_box", m_localBox);
		visitor.pushMeta(MetaData::ReadOnly());
		visitor("global_box", m_globalBox);
		visitor.popMeta();
	}
	
	bool useGravity() const { return m_bUseGravity; }
	void setGravity(bool bUseGravity) { m_bUseGravity = bUseGravity; }

	Vector3 const& getPosition() const { return m_position; }
	Vector3& getPosition() { return m_position; }
	void setPosition(Vector3 const& position);

	Vector3 const& getVelocity() const { return m_velocity; }
	Vector3& getVelocity() { return m_velocity; }
	void setVelocity(Vector3 const& velocity) { m_velocity = velocity; };

	AxisAlignedBox const& getLocalBox() const { return m_localBox; }
	void setLocalBox(AxisAlignedBox const& localBox);

	Shape* getShape() const { return m_pShape; }
	void setShape(Shape* pShape) { m_pShape = pShape; }
	bool hasShape() const { return m_pShape != nullptr; }

	AxisAlignedBox const& getGlobalBox() { return m_globalBox; }

	void update(TTimePhys deltaT);
};