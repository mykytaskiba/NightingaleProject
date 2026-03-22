#pragma once
#include "json.hpp"

enum class JSONMode {
	Serialize = 0,
	Deserialize = 1,

};

struct JSONOperation {
	JSONMode m_mode;
	nlohmann::json& m_json;

	template<typename T>
	void link(std::string const& key, T& value) {
		if (m_mode == JSONMode::Serialize) {
			m_json[key] = value;
		} 
		else if (m_mode == JSONMode::Deserialize) {
			if (m_json.contains(key)) {
				value = m_json[key].get<T>();
			}
		}
	}

	template <typename T>
	void serialize_only(std::string const& key, T const& value) {
		if (m_mode == JSONMode::Serialize) {
			m_json[key] = value;
		}
	}

	template <typename T>
	void deserialize_only(std::string const& key, T& value) {
		if (m_mode == JSONMode::Deserialize) {
			link(key, value);
		}
	}
};

struct JSONUpgrader {

	nlohmann::json& m_json;
	uint m_fileVersion;

	JSONUpgrader(nlohmann::json& json, uint version) : m_json(json), m_fileVersion(version) {}

	bool deleted_variable(uint version, std::string const& key) {
		if (m_fileVersion == version) {
			if (m_json.contains(key)) {
				m_json.erase(key);
				return true;
			}
			return false;
		}
		return true;
	}

	bool renamed_variable(uint version, std::string const& original_key, std::string const& renamed_key) {
		if (m_fileVersion == version) {
			if (m_json.contains(original_key) && !m_json.contains(renamed_key)) {
				m_json[renamed_key] = m_json[original_key];
				m_json.erase(original_key);
				return true;
			}
			return false;
		}
		return true;
	}

	template <typename TVal>
	bool added_variable(uint version, std::string const& key, TVal const& val) {
		if (m_fileVersion == version) {
			if (!m_json.contains(key)) {
				m_json[key] = val;
				return true;
			}
			return false;
		}
		return true;
	}


};

inline constexpr const char* JSON_META_KEY = "_meta_";
inline constexpr const char* JSON_VERSION_KEY = "version";
inline constexpr const char* JSON_TYPE_KEY = "type";

template <typename TTarget, unsigned int CVersion, const char* CType>
class JSONRepresentation {

private:
	TTarget& m_target;

	static constexpr const char* s_type = CType;
	static constexpr const uint s_version = CVersion;
	nlohmann::json m_json{};

public:
	JSONRepresentation(TTarget& target) : m_target(target) {}
	JSONRepresentation(TTarget& target, nlohmann::json const& jsonSource) : m_target(target), m_json(jsonSource) {}

	void executeOperation(JSONMode mode) {
		//before the of the links, see if we need to see if we need to convert to our version
		if (mode == JSONMode::Deserialize) {
			if (!upgradeJSON()) {
				//Couldnt update JSON
				return;
			}
		}

		JSONOperation operation{ mode, m_json };
		
		if (mode == JSONMode::Serialize) {
			JSONOperation metaOP{ mode, m_json[JSON_META_KEY] };
			metaOP.serialize_only(JSON_VERSION_KEY,s_version);
			metaOP.serialize_only(JSON_TYPE_KEY,s_type);
		}

		m_target.jsonOperation(operation);
	}

	bool upgradeJSON() {
		if (!m_json.contains(JSON_META_KEY)) return false;
		nlohmann::json& metaJSON = m_json[JSON_META_KEY];
		if (!metaJSON.contains(JSON_VERSION_KEY)) return false;
		if (!metaJSON.contains(JSON_TYPE_KEY)) return false;

		std::string type = metaJSON[JSON_TYPE_KEY].get<std::string>();
		if (type != s_type) {
			return false;
		}

		uint fileVersion = metaJSON[JSON_VERSION_KEY].get<uint>();
		if (fileVersion > s_version) {
			return false;
		}
		if (fileVersion < s_version) {
			JSONUpgrader upgrader(m_json, fileVersion);
			while (fileVersion < s_version) {
				bool bVersionJumpSuccess = TTarget::upgradeJSON(upgrader);
				if (!bVersionJumpSuccess) {
					return false;
				}
				++fileVersion;
				upgrader.m_fileVersion = fileVersion;
			}
		}
		if (fileVersion == s_version) {
			m_json[JSON_META_KEY][JSON_VERSION_KEY] = fileVersion;
			return true;
		}
		assert(false);
		return false;
	}

	//TTarget must implement functions
	//member bool jsonOperation(JSONOperation& operation);
	//static bool TTarget::upgradeJSON(JSONUpgrader&);

	nlohmann::json const& getJSON() const {
		return m_json;
	}
};