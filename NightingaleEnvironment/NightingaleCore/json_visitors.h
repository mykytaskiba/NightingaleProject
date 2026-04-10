#pragma once
#include "factory.h"
#include "json.hpp"
#include "json_key_constants.h"
#include "property_provider.h"
#include "property_visitor.h"
#include "service_locator.h"
#include <vector>

struct JSONSerializerVisitor : public IPropertyVisitor {

	JSONSerializerVisitor(nlohmann::json& json) : m_json(json) { }

	void operator()(std::string const& key, bool& value) override { m_json[key] = value; }
	void operator()(std::string const& key, int& value) override { m_json[key] = value; }
	void operator()(std::string const& key, float& value) override { m_json[key] = value; }
	void operator()(std::string const& key, unsigned int& value) override { m_json[key] = value; }
	void operator()(std::string const& key, std::string& value) override { m_json[key] = value; }
	void operator()(std::string const& key, Color& value) override { m_json[key] = value; }
	void operator()(std::string const& key, Vector2& value) override { m_json[key] = value; }
	void operator()(std::string const& key, Vector3& value) override { m_json[key] = value; }
	void operator()(std::string const& key, Vector4& value) override { m_json[key] = value; }

protected:
	nlohmann::json& m_json;

	std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key) override;
	std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, size_t& size) override;
	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory) override;

	void handleFactoryInternal(nlohmann::json& json, IFactoryElement*& pValue, IFactory& factory);
};

struct JSONSerializerCollectionVisitor : public JSONSerializerVisitor {

	JSONSerializerCollectionVisitor(nlohmann::json& json) : JSONSerializerVisitor(json) { }

	void operator()(std::string const& key, bool& value) override { m_json.push_back(value); }
	void operator()(std::string const& key, int& value) override { m_json.push_back(value); }
	void operator()(std::string const& key, float& value) override { m_json.push_back(value); }
	void operator()(std::string const& key, unsigned int& value) override { m_json.push_back(value); }
	void operator()(std::string const& key, std::string& value) override { m_json.push_back(value); }
	void operator()(std::string const& key, Color& value) override { m_json.push_back(value); }
	void operator()(std::string const& key, Vector2& value) override { m_json.push_back(value); }
	void operator()(std::string const& key, Vector3& value) override { m_json.push_back(value); }
	void operator()(std::string const& key, Vector4& value) override { m_json.push_back(value); }

protected:
	std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key) override;
	std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, size_t& size) override;

	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory) override;

};


struct JSONDeserializerVisitor : public IPropertyVisitor {

	JSONDeserializerVisitor(nlohmann::json& json) : m_json(json) { }

	void operator()(std::string const& key, bool& value) override { if (m_json.contains(key)) m_json[key].get_to(value); }
	void operator()(std::string const& key, int& value) override { if (m_json.contains(key)) m_json[key].get_to(value); }
	void operator()(std::string const& key, float& value) override {	if (m_json.contains(key)) m_json[key].get_to(value); }
	void operator()(std::string const& key, unsigned int& value) override { if (m_json.contains(key)) m_json[key].get_to(value); }
	void operator()(std::string const& key, std::string& value) override { if (m_json.contains(key)) m_json[key].get_to(value);  }
	void operator()(std::string const& key, Color& value) override { if (m_json.contains(key)) m_json[key].get_to(value); }
	void operator()(std::string const& key, Vector2& value) override { if (m_json.contains(key)) m_json[key].get_to(value); }
	void operator()(std::string const& key, Vector3& value) override { if (m_json.contains(key)) m_json[key].get_to(value); }
	void operator()(std::string const& key, Vector4& value) override { if (m_json.contains(key)) m_json[key].get_to(value); }


protected:
	nlohmann::json& m_json;

	std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key) override;
	std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, size_t& size) override;
	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory) override;

	void handleFactoryInternal(nlohmann::json& json, IFactoryElement*& pValue, IFactory& factory);
};


struct JSONDeserializerCollectionVisitor : public JSONDeserializerVisitor {
	JSONDeserializerCollectionVisitor(nlohmann::json& json, size_t size) : JSONDeserializerVisitor(json), m_size(size) {}

	size_t m_index{ 0u };
	size_t m_size{ 0u };

	void operator()(std::string const& key, bool& value) override
	{
		if (m_index < m_size) m_json.at(m_index++).get_to(value);
	}
	void operator()(std::string const& key, int& value) override
	{
		if (m_index < m_size) m_json.at(m_index++).get_to(value);
	}
	void operator()(std::string const& key, float& value) override
	{
		if (m_index < m_size) m_json.at(m_index++).get_to(value);
	}
	void operator()(std::string const& key, unsigned int& value) override
	{
		if (m_index < m_size) m_json.at(m_index++).get_to(value);
	}
	void operator()(std::string const& key, std::string& value) override
	{
		if (m_index < m_size) m_json.at(m_index++).get_to(value);
	}
	void operator()(std::string const& key, Color& value) override
	{
		if (m_index < m_size) m_json.at(m_index++).get_to(value);
	}
	void operator()(std::string const& key, Vector2& value) override
	{
		if (m_index < m_size) m_json.at(m_index++).get_to(value);
	}
	void operator()(std::string const& key, Vector3& value) override
	{
		if (m_index < m_size) m_json.at(m_index++).get_to(value);
	}
	void operator()(std::string const& key, Vector4& value) override
	{
		if (m_index < m_size) m_json.at(m_index++).get_to(value);
	}


protected:

	std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key) override;
	std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, size_t& size) override;
	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory) override;
};