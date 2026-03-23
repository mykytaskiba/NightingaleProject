#pragma once
#include "json.hpp"
#include "properties.h"

struct JSONSerializerVisitor {
	nlohmann::json& m_json;

	template<typename TValue, typename... TMeta >
	void operator()(std::string const& key, TValue& value, TMeta&&... meta) {
		m_json[key] = value;
	}

	template<typename TValue, typename... TMeta >
	requires HasProperties<TValue>
	void operator()(std::string const& key, TValue& value, TMeta&&... meta) {
		JSONSerializerVisitor childSerializer{ m_json[key] };
		value.properties(childSerializer);
	}

};

struct JSONDeserializerVisitor {
	nlohmann::json const& m_json;

	template<typename TValue, typename... TMeta >
	void operator()(std::string const& key, TValue& value, TMeta&&... meta) {
		if (Meta::has<Meta::ReadOnly>(meta...)) {
			return;
		}
		if (m_json.contains(key)) {
			value = m_json[key].get<TValue>();
		}
	}

	template<typename TValue, typename... TMeta >
	requires HasProperties<TValue>
		void operator()(std::string const& key, TValue& value, TMeta&&... meta) {
		if (Meta::has<Meta::ReadOnly>(meta...)) {
			return;
		}

		if (m_json.contains(key)) {
			JSONDeserializerVisitor childSerializer{ m_json[key] };
			value.properties(childSerializer);
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

	static constexpr const char* s_type = CType;
	static constexpr const uint s_version = CVersion;

public:
	JSONRepresentation() {}
	

	static 
	nlohmann::json serialize(TTarget& target) {
		nlohmann::json json;

		//Meta Data
		json[JSON_META_KEY][JSON_VERSION_KEY] = s_version;
		json[JSON_META_KEY][JSON_TYPE_KEY] = s_type;

		JSONSerializerVisitor serializer{ json };
		target.properties(serializer);

		return json;
	}

	static
	void deserialize(nlohmann::json& json, TTarget& target) {
		if (!upgradeJSON(json)) {
			//Couldnt update JSON
			return;
		}

		JSONDeserializerVisitor deserializer{ json };
		target.properties(deserializer);
	}

	static
	bool upgradeJSON(nlohmann::json& json) {
		if (!json.contains(JSON_META_KEY)) return false;
		nlohmann::json& metaJSON = json[JSON_META_KEY];
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
			JSONUpgrader upgrader(json, fileVersion);
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
			json[JSON_META_KEY][JSON_VERSION_KEY] = fileVersion;
			return true;
		}
		assert(false);
		return false;
	}

	//TTarget must implement functions
	//member bool jsonOperation(JSONOperation& operation);
	//static bool TTarget::upgradeJSON(JSONUpgrader&);
};

//Argument serialization/deserialization section
//====================================================

//NGMath is safe to include here
#include "ngmath.h"

void to_json(nlohmann::json& json, const Vector3& vec);
void from_json(nlohmann::json const& json, Vector3& vec);

void to_json(nlohmann::json& json, const Vector4& vec);
void from_json(nlohmann::json const& json, Vector4& vec);

void to_json(nlohmann::json& json, const Quaternion& quat);
void from_json(nlohmann::json const& json, Quaternion& quat);
