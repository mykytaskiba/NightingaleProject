#pragma once
#include <string>
#include "property_metadata.h"
#include "property_provider.h"
#include "factory.h"
#include <stack>
#include "json.hpp"
#include "color.h"
#include "vector.h"
#include "quaternion.h"

class IPropertyVisitor {
public:
	//Base Types
	virtual void operator()(std::string const& key, bool& value) = 0;
	virtual void operator()(std::string const& key, int& value) = 0;
	virtual void operator()(std::string const& key, float& value) = 0;
	virtual void operator()(std::string const& key, unsigned int& value) = 0;
	virtual void operator()(std::string const& key, std::string& value) = 0;
	virtual void operator()(std::string const& key, Color& value) = 0;
	virtual void operator()(std::string const& key, Vector2& value) = 0;
	virtual void operator()(std::string const& key, Vector3& value) = 0;
	virtual void operator()(std::string const& key, Vector4& value) = 0;
	virtual void operator()(std::string const& key, Quaternion& value) = 0;

	template<typename TValue>
	void operator()(std::string const& key, TValue& value) {
		//static_assert(false, "Visitor definition couldnt be resolved");
		assert(false);
	}

	template<typename TValue>
	requires HasProperties<TValue>
		void operator()(std::string const& key, TValue& value) {
		std::unique_ptr<IPropertyVisitor> pChild = childVisitor(key);
		if (pChild != nullptr) {
			pChild->pushMeta(meta());
			value.properties(*pChild); 
			pChild->popMeta();
			endChild(key);
		}
	}

	template<typename TValue>
	void operator()(std::string const& key, std::vector<TValue>& vector) {
		size_t vecSize = vector.size();
		std::unique_ptr<IPropertyVisitor> pChild = collectionVisitor(key, vecSize);
		vector.resize(vecSize);
		if (pChild != nullptr) {
			pChild->pushMeta(meta());
			unsigned int idx{ 0u };
			for (auto& value : vector) {
				pChild->enumerateCollectionItem(idx++);
				(*pChild)(key, value);
				pChild->endCollectionItem();
			}
			pChild->popMeta();
			endCollection(key);
		}
	}

	template<typename TValue>
	requires IsFactoryObject<TValue>&& HasProperties<TValue>
		void operator()(std::string const& key, TValue*& pValue) {

		if (!ServiceLocator<Factory<TValue>>::hasService()) {
			assert(false);
			return;
		}
		Factory<TValue>& factory = *ServiceLocator<Factory<TValue>>::retrieve();
		IFactoryElement* pFactoryElement = pValue;
		handleFactory(key, pFactoryElement, factory);
		pValue = dynamic_cast<TValue*>(pFactoryElement);
	}

	//Meta Data handling
	void pushMeta(MetaData const& metaData) {
		if (m_metaStack.empty()) {
			m_metaStack.push(metaData);
			return;
		}

		m_metaStack.push(m_metaStack.top() | metaData);
	}

	void popMeta() {
		if (m_metaStack.empty()) {
			assert(false);
		}
		m_metaStack.pop();
	}


protected:

	std::stack<MetaData> m_metaStack{};
	MetaData nullMeta{};

	MetaData const& meta() {
		if (m_metaStack.empty()) {
			return nullMeta;
		}
		return m_metaStack.top();
	}


	virtual std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key) = 0;
	virtual void endChild(std::string const& key) {}

	virtual std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, size_t& size) = 0;
	virtual void endCollection(std::string const& key) {}

	virtual void enumerateCollectionItem(unsigned int id) {}
	virtual void endCollectionItem() {}

	virtual void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory) = 0;

	virtual void handle_quaternion(std::string const& key, float& w, float& x, float& y, float& z) {
		std::unique_ptr<IPropertyVisitor> pChild = childVisitor(key);
		if (pChild != nullptr) {
			pChild->pushMeta(meta());
			(*pChild)("w", w);
			(*pChild)("x", x);
			(*pChild)("y", y);
			(*pChild)("z", z);
			pChild->popMeta();
			endChild(key);
		}
	}

};
