#pragma once
class DebugPanel {

public:
	void register_panel();
	void deregister_panel();

	virtual void init() {};
	virtual void render_update() {};

};