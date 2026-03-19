#pragma once

class GameObject;
class PhysicsBody;

class PhysicsBodyMenu {
public:
	static void render_update(GameObject& gameObject);
	static void render_update(PhysicsBody& pPhysBody);
};