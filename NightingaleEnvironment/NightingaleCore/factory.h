#pragma once
#include <map>
#include <functional>
#include "nightingale_assert.h"
#include "service_locator.h"


class IFactoryElement {
	template <typename TValue>
	friend class Factory;
	using TFactoryKey = std::string;
private:
	void setFactoryKey(TFactoryKey const& key) {
		m_factoryKey = key;
	}

	TFactoryKey m_factoryKey{};
public:
	TFactoryKey const& getFactoryKey() const { return m_factoryKey; }
	virtual ~IFactoryElement() = default; //make this class virtual
};

class IFactory {
public:
	using TFactoryKey = std::string;
	virtual bool create(TFactoryKey const& key, IFactoryElement*& outValue) const = 0;
};



template <typename TValue>
class Factory : public IFactory {
private:
	using FKeyFunc = std::function<void(TFactoryKey const&)>;
	using FCreationFunc = std::function<TValue* ()>;
	std::map<TFactoryKey, FCreationFunc> m_map{};
public:

	Factory() {
		ServiceLocator<Factory<TValue>>::provide(this);
	}
	~Factory() {
		ServiceLocator<Factory<TValue>>::clear();
	}

	Factory(const Factory&) = delete;
	Factory& operator=(Factory const&) = delete;

	void addToFactory(TFactoryKey const& key, FCreationFunc creationFunc) {
		if (m_map.contains(key)) {
			assert(false && "Value already exists in the factory");
			return;
		}
		m_map[key] = std::move(creationFunc);
	}

	//Assumption: TValue added must have a no param constructor
	template <typename TValueAdded> requires std::derived_from<TValueAdded, TValue>
	void addToFactory(TFactoryKey const& key) {
		addToFactory(key, []() {
			return new TValueAdded();
			}
		);
	}

	bool create(TFactoryKey const& key, IFactoryElement*& pOut) const override {

		if (pOut != nullptr) {
			assert(false); //Must pass nullptr into factory
			return false;
		}

		auto it = m_map.find(key);
		if (it == m_map.end()) {
			return false;
		}

		pOut = it->second();

		if (pOut == nullptr) {
			assert(false);
			return false;
		}

		pOut->setFactoryKey(key);
		return true;
	}

	bool create(TFactoryKey const& key, TValue*& pOut) const {
		if (pOut != nullptr) {
			assert(false); //Must pass nullptr into factory
			return false;
		}

		IFactoryElement* pOutElement{ nullptr };
		if (create(key, pOutElement)) {
			// Since this is a Factory<TValue>, we know pBaseOut is a TValue
			pOut = static_cast<TValue*>(pOutElement);
			return true;
		}
		return false;
	}

	void foreach_key(FKeyFunc const& func) const {
		for (auto const&[key, creationFunc] : m_map) {
			func(key);
		}
	}
};


template <typename TObject>
concept IsFactoryObject = std::derived_from<TObject, IFactoryElement>;