#pragma once
#include "json.hpp"
#include "json_visitors.h"
#include "json_upgrader.h"
#include "json_object.h"
#include "json_key_constants.h"
#include "json_representation.h"
#include "property_provider.h"
#include "factory.h"
#include "properties.h"


template <typename TTarget>
requires std::derived_from<TTarget, IJSONObject>
class JSONRepresentation : public IJSONRepresentation {
private:
	TTarget& m_target;
public:
	JSONRepresentation(TTarget& target) : m_target(target) {}

	bool serialize(nlohmann::json& json) override {
		//Meta Data
		json[JSON_META_KEY][JSON_VERSION_KEY] = m_target.jsonVersion();
		json[JSON_META_KEY][JSON_TYPE_KEY] = m_target.jsonType();
		if constexpr (IsFactoryObject<TTarget>)
			json[JSON_META_KEY][JSON_FACTORY_KEY] = m_target.getFactoryKey();
		

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
		if constexpr (IsFactoryObject<TTarget>)
			if (!metaJSON.contains(JSON_FACTORY_KEY)) return false;

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


