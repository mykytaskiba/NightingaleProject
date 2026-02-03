#pragma once
#include "defines.h"
#include "physics_body.h"

#define DEFAULT_PHYSICS_FRAMERATE 10 
//Main class for the physics system
class Physics {
public:
	friend class PhysicsDebugRenderPass;

	Physics();


	void update(TTimePhys frameDelta);
	void subUpdate(TTimePhys deltaT);

	void setTargetUpdateRate(uint updatesPerSecond);
	void setMaxUpdatesPerFrame(uint maxUpdatePerFrame);
	void setDiscardUnusedTime(bool bDiscardUnusedTime);

	void addBody(Body* pBody);

private:

	TTimePhys m_accumulatedTime{ 0.0f };
	TTimePhys m_updateRate{ 0.0f };

	uint m_maxUpdatesPerFrame{1u};
	bool m_bDiscardUnusedTime{ true };

	vector<Body*> m_activeBodies{};
};