#pragma once
#include "nightingale_assert.h"

struct FrameContext {
public:
	bool isValid() {
		//DO NOT SEND THE FRAME CONTEXT IF THIS RETURNS FALSE
		if (m_pScene == nullptr) {
			return false;
		}

		return true;
	}

	TTimeDelta deltaTime() { return m_deltaTime; }

	Scene& activeScene() { return *m_pScene; }

private:
	TTimeDelta m_deltaTime{ 0.0f };

	Scene* m_pScene{ nullptr };
};