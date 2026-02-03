#pragma once
#include "ngmath.h"
#include "defines.h"

//Any physics body that needs to be simulated
//Could be rigid or soft
class Body {
public:
	AxisAlignedBox globalBox;
	AxisAlignedBox localBox;

	Vector3 position{ 0.0f,0.0f,0.0f };
	Vector3 velocity{ 0.0f,0.0f,0.0f };

	bool bUseGravity{ false };

	void update(TTimePhys deltaT);
};