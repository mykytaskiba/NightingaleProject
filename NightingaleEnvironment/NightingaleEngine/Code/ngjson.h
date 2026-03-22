#pragma once

enum class JSONMode {
	Serialize = 0,
	Deserialize = 1,
	Verify = 2,
	Convert = 3

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
			T valueCpy = { value };
			link(key, valueCpy);
		}
	}

	template <typename T>
	void deserialize_only(std::string const& key, T& value) {
		if (m_mode == JSONMode::Deserialize) {
			link(key, value);
		}
	}
};
