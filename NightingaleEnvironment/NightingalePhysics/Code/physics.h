#pragma once
#include "defines.h"
#include "physics_body.h"
#include "ngmath.h"
#include "property_visitor.h"

#define DEFAULT_PHYSICS_FRAMERATE 10 
//Main class for the physics system
class Physics {
public:
	friend class PhysicsDebugRenderPass;

	Physics();


	void update(TTimePhys frameDelta);
	void subUpdate(TTimePhys deltaT);

	void addBody(PhysicsBody* pBody);
	void removeBody(PhysicsBody* pBody);

	bool isActive() const { return m_bActive; }
	void setActive(bool bActive) { m_bActive = bActive; }

	uint getMaxUpdatesPerFrame() const { return m_maxUpdatesPerFrame; }
	void setMaxUpdatesPerFrame(uint maxUpdatePerFrame) { m_maxUpdatesPerFrame = maxUpdatePerFrame; }

	bool getDiscardUnusedTime() const { return m_bDiscardUnusedTime; }
	void setDiscardUnusedTime(bool bDiscardUnusedTime) { m_bDiscardUnusedTime = bDiscardUnusedTime; }

	bool getInterpolateBetweenFrames() const { return m_bInterpolateBetweenFrames; }
	void setInterpolateBetweenFrames(bool bInterpolateBetweenFrames) { m_bInterpolateBetweenFrames = bInterpolateBetweenFrames; }

	uint getUpdatesPerSecond() const { return m_infoUpdatesPerSecond; }
	void setTargetUpdateRate(uint updatesPerSecond);

	TTimePhys getMaxAccumulatedTime() const { return m_maxAccumulatedTime; }
	void setMaxAccumulatedTime(TTimePhys maxAccumulatedTime) { m_maxAccumulatedTime = maxAccumulatedTime; }

	void addAccumulatedTime(TTimePhys time);

	TTimePhys getAccumulatedTime() const { return m_accumulatedTime; }
	TTimePhys getUpdateRate() const { return m_updateRate; }

	uint getSpatialPairsCount() const { return m_infoSpatialPairsCount; };

	void properties(IPropertyVisitor& visitor) {
		visitor("active", m_bActive);

		visitor("updates_per_second", m_infoUpdatesPerSecond, MetaData::ReadOnly);

		visitor("max_accumulated_time", m_accumulatedTime, MetaData::ReadOnly);
		visitor("update_rate", m_updateRate, MetaData::ReadOnly);

		visitor("discard_unused", m_bDiscardUnusedTime);
		visitor("interpolate_between_frames", m_bInterpolateBetweenFrames);
		visitor("max_updates_per_frame", m_maxUpdatesPerFrame);
		visitor("max_accumulated_time", m_maxAccumulatedTime);
	}

private:

	//Whether all physics are active
	bool m_bActive{ false };

	uint m_infoUpdatesPerSecond{ 0u }; //Info variable, no functional effect
	uint m_infoSpatialPairsCount{ 0u };

	TTimePhys m_updateRate{ 0.0f };
	TTimePhys m_accumulatedTime{ 0.0f };
	TTimePhys m_maxAccumulatedTime{ 100.0f };

	uint m_maxUpdatesPerFrame{1u};
	bool m_bDiscardUnusedTime{ true };
	bool m_bInterpolateBetweenFrames{ false };


	vector<PhysicsBody*> m_vActiveBodies{};
	SpatialBucket<PhysicsBody> m_spatialStructure{};
};