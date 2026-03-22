#pragma once
#include "debug_panel.h"
#include "ngjson.h"

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

	struct SampleObject {

		//Past Data (for showcase)
		//float m_deletedFloat; //v2
		//std::string m_renamedVariable; //v3
		//int m_addedInt //v4
		//int m_deletedInt; //v5
		
		int m_addedInt;
		float m_originalFloat;
		std::string m_renamedVariable;


		constexpr static char c_JSONType[] = "json.sampleobject";
		constexpr static uint c_JsonVersion = 5u;
		using JSONRepresentation = JSONRepresentation<SampleObject, c_JsonVersion, c_JSONType>;
		static bool upgradeJSON(JSONUpgrader& upgrader);
		bool jsonOperation(JSONOperation& operation);
	};
};