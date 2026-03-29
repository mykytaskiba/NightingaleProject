#pragma once
#include <string>
#include "property_metadata.h"
#include "property_provider.h"
#include "factory.h"


class IPropertyVisitor {
public:
	//Base Types
	virtual void operator()(std::string const& key, int& value, MetaData const& metaData = {}) = 0;
	virtual void operator()(std::string const& key, float& value, MetaData const& metaData = {}) = 0;
	virtual void operator()(std::string const& key, unsigned int& value, MetaData const& metaData = {}) = 0;
	virtual void operator()(std::string const& key, std::string& value, MetaData const& metaData = {}) = 0;

	template<typename TValue>
	requires HasProperties<TValue>
		void operator()(std::string const& key, TValue& value, MetaData const& metaData = {}) {
		if (enterScope(key, metaData)) {
			value.properties(*this);
			leaveScope(metaData);
		}
	}

	template<typename TValue>
	void operator()(std::string const& key, TValue& value, MetaData const& metaData = {}) {
		//static_assert(false, "Visitor definition couldnt be resolved");
		assert(false);
	}

	template<typename TValue>
	requires IsFactoryObject<TValue>&& HasProperties<TValue>
		void operator()(std::string const& key, TValue*& pValue, MetaData const& metaData = {}) {

		if (!ServiceLocator<Factory<TValue>>::hasService()) {
			assert(false);
			return;
		}
		Factory<TValue>& factory = *ServiceLocator<Factory<TValue>>::retrieve();
		IFactoryElement* pFactoryElement = pValue;
		handleFactory(key, pFactoryElement, factory, metaData);

	}

	template<typename TValue>
	void operator()(std::string const& key, std::vector<TValue>& vector, MetaData const& metaData = {}) {
		unsigned int idx{ 0u };
		size_t vectorSize = vector.size();
		if (enterCollection(key, vectorSize, metaData)) {
			vector.resize(vectorSize);
			for (auto& value : vector) {
				enumerateCollectionElement(idx, metaData);
				std::string idxKey = "[" + std::to_string(idx) + "]";
				(*this)(idxKey, value, metaData);
				++idx;
			}
			leaveCollection(metaData);
		}
	}

protected:
	virtual bool enterScope(std::string const& key, MetaData const& metaData) = 0;
	virtual void leaveScope(MetaData const& metaData) = 0;

	virtual bool enterCollection(std::string const& key, size_t& count, MetaData const& metaData) {
		return enterScope(key, metaData);
	}

	virtual void enumerateCollectionElement(unsigned int val, MetaData const& metaData) {
	}

	virtual void leaveCollection(MetaData const& metaData) {
		leaveScope(metaData);
	}

	virtual void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData) = 0;


	
	virtual void handle_vector3(std::string const& key, float& x, float& y, float& z, MetaData const& metaData) {
		if (enterScope(key, metaData)) {
			(*this)("x", x, metaData);
			(*this)("y", y, metaData);
			(*this)("z", z, metaData);
			leaveScope(metaData);
		}
	}
	virtual void handle_vector4(std::string const& key, float& x, float& y, float& z, float& w, MetaData const& metaData) {
		if (enterScope(key, metaData)) {
			(*this)("x", x, metaData);
			(*this)("y", y, metaData);
			(*this)("z", z, metaData);
			(*this)("w", w, metaData);
			leaveScope(metaData);
		}
	}
	virtual void handle_quaternion(std::string const& key, float& w, float& x, float& y, float& z, MetaData const& metaData) {
		if (enterScope(key, metaData)) {
			(*this)("w", w, metaData);
			(*this)("x", x, metaData);
			(*this)("y", y, metaData);
			(*this)("z", z, metaData);
			leaveScope(metaData);
		}
	}

};
