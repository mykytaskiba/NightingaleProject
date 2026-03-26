#pragma once
#include <string>
#include <memory>
#include "json_representation.h"

class JSONUpgrader;

class IJSONObject {
public:
	virtual std::string jsonType() const = 0;
	virtual unsigned int jsonVersion() const = 0; //std::max(c_version, parent::jsonVersion); 
	virtual std::unique_ptr<IJSONRepresentation> json() = 0;

	virtual bool jsonUpgrade(JSONUpgrader& upgrader) const { return true; };
};

#define JSON_PARENT(CLASS, VERSION, TYPE_STRING)							\
std::string jsonType() const override { return TYPE_STRING; };		\
uint jsonVersion() const override { return VERSION; }	\
std::unique_ptr<IJSONRepresentation> json() override; \
//END

#define JSON_CHILD(CLASS, VERSION, PARENT_CLASS)							\
uint jsonVersion() const override { return std::max(VERSION,PARENT_CLASS::jsonVersion()); }	\
std::unique_ptr<IJSONRepresentation> json() override; \
//END

#define JSON_IMPL(CLASS) \
std::unique_ptr<IJSONRepresentation> CLASS::json() { \
    return std::make_unique<JSONRepresentation<CLASS>>(*this); \
} \
//END

