#include "json_visitors.h"
#include "json_object.h"

std::unique_ptr<IPropertyVisitor> JSONSerializerVisitor::childVisitor(std::string const& key, MetaData const& metaData) {
	return std::make_unique<JSONSerializerVisitor>(m_json[key]);
}

std::unique_ptr<IPropertyVisitor>  JSONSerializerVisitor::collectionVisitor(std::string const& key, MetaData const& metaData) {
	return std::make_unique<JSONSerializerCollectionVisitor>(m_json[key]);
}

void JSONSerializerVisitor::handleFactoryInternal(nlohmann::json& json, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData)
{
	if (pValue == nullptr) {
		json = nlohmann::json();
		return;
	}

	IJSONObject* pJSONObject = dynamic_cast<IJSONObject*>(pValue);
	if (pJSONObject == nullptr) {
		assert(false);
		return;
	}

	bool bSuccess = pJSONObject->json()->serialize(json);
	assert(bSuccess);
}

void JSONSerializerVisitor::handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData)
{
	handleFactoryInternal(m_json[key], pValue, factory, metaData);
}

std::unique_ptr<IPropertyVisitor> JSONSerializerCollectionVisitor::childVisitor(std::string const& key, MetaData const& metaData) {
	m_json.push_back(nlohmann::json::object());
	nlohmann::json& jsonElement = m_json.back();
	return std::make_unique<JSONSerializerVisitor>(jsonElement);

}

std::unique_ptr<IPropertyVisitor>  JSONSerializerCollectionVisitor::collectionVisitor(std::string const& key, MetaData const& metaData) {
	m_json.push_back(nlohmann::json::array());
	nlohmann::json& jsonElement = m_json.back();
	return std::make_unique<JSONSerializerCollectionVisitor>(jsonElement);
}

void JSONSerializerCollectionVisitor::handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData)
{
	m_json.push_back(nlohmann::json::object());
	nlohmann::json& jsonElement = m_json.back();
	handleFactoryInternal(jsonElement, pValue, factory, metaData);
}