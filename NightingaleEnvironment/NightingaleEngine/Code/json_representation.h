#pragma once
#include "json.hpp"

class IJSONRepresentation {
public:
	virtual bool serialize(nlohmann::json& json) = 0;
	virtual bool deserialize(nlohmann::json& json) = 0;
	virtual bool upgrade(nlohmann::json& json) = 0;
};