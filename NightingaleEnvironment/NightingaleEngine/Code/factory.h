#pragma once
#include <map>
#include <functional>
#include "nightingale_assert.h"

template <typename TKey, typename TValue>
class Factory {
public:
	using FCreationFunc = std::function<TValue* ()>;

private:
	std::map<TKey, FCreationFunc> m_map{};

public:

	void addToFactory(TKey const& key, FCreationFunc creationFunc) {
		if (m_map.contains(key)) {
			assert(false && "Value already exists in the factory");
			return;
		}
		m_map[key] = std::move(creationFunc);
	}

	//Assumption: TValue added must have a no param constructor
	template <typename TValueAdded> requires std::derived_from<TValueAdded, TValue>
	void addToFactory(TKey const& key) {
		addToFactory(key, []() {
			return new TValueAdded();
			}
		);
	}

	bool create(TKey const& key, TValue*& outValue) const {

		if (outValue != nullptr) {
			assert(false && "Must pass a nullptr into factory");
			return false;
		}

		auto it = m_map.find(key);
		if (it == m_map.end()) {
			return false;
		}

		outValue = it->second();
		return true;

	}

};