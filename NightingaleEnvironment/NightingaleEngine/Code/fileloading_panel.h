#pragma once
#include "debug_panel.h"
#include "game_object.h"
#include "transform.h"

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

class JSONUpgraderPanel : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;

	std::string m_path{ "jsonTest/upgraderTest/" };

	void performConversion(std::string path);

	struct SampleObject : public IJSONObject, public IPropertyProvider {

		//Past Data (for showcase)
		//float m_deletedFloat; //v2
		//std::string m_renamedVariable; //v3
		//int m_addedInt //v4
		//int m_deletedInt; //v5
		
		int m_addedInt;
		float m_originalFloat;
		std::string m_renamedVariable;

		JSON_PARENT(SampleObject, 5u, "json.sample")
		bool jsonUpgrade(JSONUpgrader& upgrader) const override;

		void properties(IPropertyVisitor& visitor) override {
			visitor("added_int", m_addedInt);
			visitor("original_float", m_originalFloat);
			visitor("renamed_string_var", m_renamedVariable);
		}
	};
};