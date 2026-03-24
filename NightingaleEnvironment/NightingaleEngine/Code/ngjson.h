#pragma once
#include "json.hpp"
#include "properties.h"
#include "json_visitors.h"
#include "json_upgrader.h"
#include "json_object.h"
#include "json_representation.h"

inline constexpr const char* JSON_META_KEY = "_meta_";
inline constexpr const char* JSON_VERSION_KEY = "version";
inline constexpr const char* JSON_TYPE_KEY = "type";


template <typename TTarget>
requires std::derived_from<TTarget, IJSONObject> && HasProperties<TTarget>
class JSONRepresentation : public IJSONRepresentation {
private:
	TTarget& m_target;
public:
	JSONRepresentation(TTarget& target) : m_target(target) {}

	bool serialize(nlohmann::json& json) override {
		//Meta Data
		json[JSON_META_KEY][JSON_VERSION_KEY] = m_target.jsonVersion();
		json[JSON_META_KEY][JSON_TYPE_KEY] = m_target.jsonType();

		JSONSerializerVisitor serializer{ json };
		m_target.properties(serializer);

		return true;
	}

	bool deserialize(nlohmann::json& json) override {
		if (!upgrade(json)) {
			//Couldnt upgrade JSON
			return false;
		}

		JSONDeserializerVisitor deserializer{ json };
		m_target.properties(deserializer);

		return true;
	}


	bool upgrade(nlohmann::json& json) override {
		std::string type = m_target.jsonType();
		uint version = m_target.jsonVersion();

		if (!json.contains(JSON_META_KEY)) return false;
		nlohmann::json& metaJSON = json[JSON_META_KEY];
		if (!metaJSON.contains(JSON_VERSION_KEY)) return false;
		if (!metaJSON.contains(JSON_TYPE_KEY)) return false;

		std::string fileType = metaJSON[JSON_TYPE_KEY].get<std::string>();
		if (fileType != type) {
			return false;
		}

		uint fileVersion = metaJSON[JSON_VERSION_KEY].get<uint>();

		if (fileVersion > version) {
			return false;
		}

		if (fileVersion < version) {
			JSONUpgrader upgrader(json, fileVersion);
			while (fileVersion < version) {
				if (!m_target.jsonUpgrade(upgrader)) {
					return false;
				}
				++fileVersion;
				upgrader.m_fileVersion = fileVersion;
			}
		}
		if (fileVersion == version) {
			json[JSON_META_KEY][JSON_VERSION_KEY] = fileVersion;
			return true;
		}

		assert(false);
		return false;
	}
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
