#pragma once
#include "ngmath.h"
#include "defines.h"

//Any physics body that needs to be simulated
//Could be rigid or soft
class PhysicsBody {
private:

	bool m_bUseGravity{ false };
	
	Vector3 m_position{ 0.0f,0.0f,0.0f };
	Vector3 m_velocity{ 0.0f,0.0f,0.0f };


	AxisAlignedBox m_globalBox{};
	AxisAlignedBox m_localBox{};

	void calculateGlobalBox();
public:

	
	bool useGravity() const { return m_bUseGravity; }
	void setGravity(bool bUseGravity) { m_bUseGravity = bUseGravity; }

	Vector3 const& getPosition() const { return m_position; }
	Vector3& getPosition() { return m_position; }
	void setPosition(Vector3 const& position) { m_position = position; }

	Vector3 const& getVelocity() const { return m_velocity; }
	Vector3& getVelocity() { return m_velocity; }
	void setVelocity(Vector3 const& velocity) { m_velocity = velocity; };

	AxisAlignedBox const& getLocalBox() const { return m_localBox; }
	void setLocalBox(AxisAlignedBox const& localBox);


	AxisAlignedBox const& getGlobalBox() { return m_globalBox; }

	void update(TTimePhys deltaT);
};