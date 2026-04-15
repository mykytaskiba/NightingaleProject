#pragma once
#include "debug_panel.h"

class PhysicsDebugRenderPass;

class PhysicsControlPanel : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;

private:
	PhysicsDebugRenderPass* findDebugPass();

	int m_simulationFrames{ 1u };
	float m_simulationTime{ 1.0f };
	float m_addedAccumulationTime{ 1.0f };
	
	void forceSimulatePhysics(float simulateTime);

	void caseSingleSphere();
	void caseSphereExplosion();
};