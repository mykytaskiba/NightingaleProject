#pragma once
#include "json.hpp"

struct JSONUpgrader {

	nlohmann::json& m_json;
	unsigned int m_fileVersion;

	JSONUpgrader(nlohmann::json& json, unsigned int version) : m_json(json), m_fileVersion(version) {}

	bool deleted_variable(unsigned int version, std::string const& key) {
		if (m_fileVersion == version) {
			if (m_json.contains(key)) {
				m_json.erase(key);
				return true;
			}
			return false;
		}
		return true;
	}

	bool renamed_variable(unsigned int version, std::string const& original_key, std::string const& renamed_key) {
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
	bool added_variable(unsigned int version, std::string const& key, TVal const& val) {
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