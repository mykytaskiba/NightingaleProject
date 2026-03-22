#pragma once
#include "debug_panel.h"

class FileSavingPanel : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;

	std::string m_contents;
	std::string m_path;
	bool m_bCreateDirectories{ true };
	bool m_bOverwriteFile{ true };

	bool m_bLastResult{ false };
};