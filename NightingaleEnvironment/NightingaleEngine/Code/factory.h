#pragma once
#include <map>
#include <functional>
#include "nightingale_assert.h"

template <typename TKey, typename TValue>
class FactoryElement {
	template <typename TKey, typename TValue>
	friend class Factory;
private:
	void setFactoryKey(TKey const& key) {
		m_factoryKey = key;
	}

	TKey m_factoryKey{};
public:
	TKey const& getFactoryKey() const { return m_factoryKey; }
};


template <typename TKey, typename TValue>
class Factory {
public:
	using FCreationFunc = std::function<TValue* ()>;
	using FKeyFunc = std::function<void(TKey const&)>;

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

		if (outValue == nullptr) {
			assert(false);
			return false;
		}

		outValue->setFactoryKey(key);
		return true;
	}

	void foreach_key(FKeyFunc const& func) const {
		for (auto const&[key, creationFunc] : m_map) {
			func(key);
		}
	}
};