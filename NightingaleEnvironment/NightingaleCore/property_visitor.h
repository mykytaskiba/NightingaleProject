#pragma once
#include <string>
#include "property_metadata.h"
#include "property_provider.h"
#include "factory.h"

class IPropertyVisitor {
public:
	//Base Types
	virtual void operator()(std::string const& key, bool& value, MetaData const& metaData = {}) = 0;
	virtual void operator()(std::string const& key, int& value, MetaData const& metaData = {}) = 0;
	virtual void operator()(std::string const& key, float& value, MetaData const& metaData = {}) = 0;
	virtual void operator()(std::string const& key, unsigned int& value, MetaData const& metaData = {}) = 0;
	virtual void operator()(std::string const& key, std::string& value, MetaData const& metaData = {}) = 0;
	//virtual void operator()(std::string const& )

	template<typename TValue>
	void operator()(std::string const& key, TValue& value, MetaData const& metaData = {}) {
		//static_assert(false, "Visitor definition couldnt be resolved");
		//assert(false);
	}

	template<typename TValue>
	requires HasProperties<TValue>
		void operator()(std::string const& key, TValue& value, MetaData const& metaData = {}) {
		std::unique_ptr<IPropertyVisitor> pChild = childVisitor(key, metaData);
		if (pChild != nullptr) {
			value.properties(*pChild); 
			endChild(key);
		}
	}

	template<typename TValue>
	void operator()(std::string const& key, std::vector<TValue>& vector, MetaData const& metaData = {}) {
		size_t vecSize = vector.size();
		std::unique_ptr<IPropertyVisitor> pChild = collectionVisitor(key, vecSize, metaData);
		vector.resize(vecSize);
		if (pChild != nullptr) {
			for (auto& value : vector) {
				(*pChild)(key, value, metaData);
			}
			endCollection(key);
		}
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
		pValue = dynamic_cast<TValue*>(pFactoryElement);
	}


protected:
	virtual std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key, MetaData const& metaData) = 0;
	virtual void endChild(std::string const& key) {}

	virtual std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, size_t& size, MetaData const& metaData) = 0;
	virtual void endCollection(std::string const& key) {}

	virtual void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData) = 0;

	virtual void handle_vector3(std::string const& key, float& x, float& y, float& z, MetaData const& metaData) {
		std::unique_ptr<IPropertyVisitor> pChild = childVisitor(key, metaData);
		if (pChild != nullptr) {
			(*pChild)("x", x, metaData);
			(*pChild)("y", y, metaData);
			(*pChild)("z", z, metaData);
			endChild(key);
		}
	}
	virtual void handle_vector4(std::string const& key, float& x, float& y, float& z, float& w, MetaData const& metaData) {
		std::unique_ptr<IPropertyVisitor> pChild = childVisitor(key, metaData);
		if (pChild != nullptr) {
			(*pChild)("x", x, metaData);
			(*pChild)("y", y, metaData);
			(*pChild)("z", z, metaData);
			(*pChild)("w", w, metaData);
			endChild(key);
		}
	}
	virtual void handle_quaternion(std::string const& key, float& w, float& x, float& y, float& z, MetaData const& metaData) {
		std::unique_ptr<IPropertyVisitor> pChild = childVisitor(key, metaData);
		if (pChild != nullptr) {
			(*pChild)("w", w, metaData);
			(*pChild)("x", x, metaData);
			(*pChild)("y", y, metaData);
			(*pChild)("z", z, metaData);
			endChild(key);
		}
	}

};
