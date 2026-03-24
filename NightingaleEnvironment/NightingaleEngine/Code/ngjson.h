#pragma once
#include "json.hpp"
#include "properties.h"
#include "serializer_visitor.h"
#include "deserializer_visitor.h"
#include "json_upgrader.h"

inline constexpr const char* JSON_META_KEY = "_meta_";
inline constexpr const char* JSON_VERSION_KEY = "version";
inline constexpr const char* JSON_TYPE_KEY = "type";


class IJSONRepresentation {
public:
	virtual bool serialize(nlohmann::json& json) = 0;
	virtual bool deserialize(nlohmann::json& json) = 0;
	virtual bool upgrade(nlohmann::json& json) = 0;
};

class IJSONObject {
public:
	virtual std::string jsonType() const = 0;
	virtual uint jsonVersion() const = 0; //std::max(c_version, parent::jsonVersion); 
	virtual bool jsonUpgrade(JSONUpgrader& upgrader) const { return true; };
	virtual std::unique_ptr<IJSONRepresentation> json() = 0;
};


#define JSON_PARENT(CLASS, VERSION, TYPE_STRING)							\
std::string jsonType() const override { return TYPE_STRING; };		\
uint jsonVersion() const override { return VERSION; }	\
std::unique_ptr<IJSONRepresentation> json() override { return std::make_unique<JSONRepresentation<CLASS>>(*this); } \
//END

#define JSON_CHILD(CLASS, VERSION, PARENT_CLASS)							\
uint jsonVersion() const override { return std::max(VERSION,PARENT_CLASS::jsonVersion()); }	\
std::unique_ptr<IJSONRepresentation> json() override { return std::make_unique<JSONRepresentation<CLASS>>(*this); } \
//END


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
