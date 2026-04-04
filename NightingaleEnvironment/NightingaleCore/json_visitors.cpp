#include "json_visitors.h"
#include "json_object.h"

std::unique_ptr<IPropertyVisitor> JSONSerializerVisitor::childVisitor(std::string const& key) {
	return std::make_unique<JSONSerializerVisitor>(m_json[key]);
}

std::unique_ptr<IPropertyVisitor>  JSONSerializerVisitor::collectionVisitor(std::string const& key, size_t& size) {
	return std::make_unique<JSONSerializerCollectionVisitor>(m_json[key]);
}

void JSONSerializerVisitor::handleFactoryInternal(nlohmann::json& json, IFactoryElement*& pValue, IFactory& factory)
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

void JSONSerializerVisitor::handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory)
{
	handleFactoryInternal(m_json[key], pValue, factory);
}

std::unique_ptr<IPropertyVisitor> JSONSerializerCollectionVisitor::childVisitor(std::string const& key) {
	m_json.push_back(nlohmann::json::object());
	nlohmann::json& jsonElement = m_json.back();
	return std::make_unique<JSONSerializerVisitor>(jsonElement);

}

std::unique_ptr<IPropertyVisitor>  JSONSerializerCollectionVisitor::collectionVisitor(std::string const& key, size_t& size) {
	m_json.push_back(nlohmann::json::array());
	nlohmann::json& jsonElement = m_json.back();
	return std::make_unique<JSONSerializerCollectionVisitor>(jsonElement);
}

void JSONSerializerCollectionVisitor::handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory)
{
	m_json.push_back(nlohmann::json::object());
	nlohmann::json& jsonElement = m_json.back();
	handleFactoryInternal(jsonElement, pValue, factory);
}

std::unique_ptr<IPropertyVisitor> JSONDeserializerVisitor::childVisitor(std::string const& key)
{
	if (!m_json.contains(key)) return nullptr;
	return std::make_unique<JSONDeserializerVisitor>(m_json[key]);
}

std::unique_ptr<IPropertyVisitor> JSONDeserializerVisitor::collectionVisitor(std::string const& key, size_t& size)
{
	if (!m_json.contains(key)) return nullptr;
	size = m_json[key].size();
	return std::make_unique<JSONDeserializerCollectionVisitor>(m_json[key], size);
}

void JSONDeserializerVisitor::handleFactoryInternal(nlohmann::json& json, IFactoryElement*& pValue, IFactory& factory)
{
	if (pValue != nullptr) {
		//TO DO: enable this
		//assert(false);
		//return;
	}
	
	if (!json.contains(JSON_META_KEY)) {
		return;
	}
	if (!json[JSON_META_KEY].contains(JSON_FACTORY_KEY)) {
		return;
	}
	std::string factoryKey;
	json[JSON_META_KEY][JSON_FACTORY_KEY].get_to(factoryKey);

	pValue = nullptr;
	factory.create(factoryKey, pValue);

	if (pValue == nullptr) {
		assert(false);
		return;
	}

	IJSONObject* pJSONObject = dynamic_cast<IJSONObject*>(pValue);
	if (pJSONObject == nullptr) {
		assert(false);
		return;
	}

	bool bSuccess = pJSONObject->json()->deserialize(json);
	assert(bSuccess);
}

void JSONDeserializerVisitor::handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory)
{
	if (!m_json.contains(key)) {
		return;
	}
	handleFactoryInternal(m_json[key], pValue, factory);
}


std::unique_ptr<IPropertyVisitor> JSONDeserializerCollectionVisitor::childVisitor(std::string const& key)
{
	if (m_index >= m_size) return nullptr;
	nlohmann::json& json = m_json.at(m_index++);
	return std::make_unique<JSONDeserializerVisitor>(json);
}

std::unique_ptr<IPropertyVisitor> JSONDeserializerCollectionVisitor::collectionVisitor(std::string const& key, size_t& size)
{
	if (m_index >= m_size) return nullptr;
	nlohmann::json& json = m_json.at(m_index++);
	size = json.size();
	return std::make_unique<JSONDeserializerCollectionVisitor>(json, size);
}

void JSONDeserializerCollectionVisitor::handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory)
{
	if (m_index >= m_size) return;
	nlohmann::json& json = m_json.at(m_index++);
	handleFactoryInternal(json, pValue, factory);
}
