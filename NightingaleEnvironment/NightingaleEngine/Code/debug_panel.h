#pragma once
#include "callback_handler.h"

class DebugPanel {
public:
	virtual void activate() {};
	virtual void deactivate() {};
	virtual void render_update() {};

	void toggle();
	void toggle_set(bool bState);

protected:
	bool m_bActive{ false };
	CallbackRef m_updateLoopRef{};
};