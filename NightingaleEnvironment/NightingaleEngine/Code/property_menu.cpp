#include "pch.h"
#include "property_menu.h"
#include "property_provider.h"
#include "game_object.h"

std::unique_ptr<IPropertyVisitor> PropertyMenuVisitor::childVisitor(std::string const& key, MetaData const& metaData) {
	if (ImGui::TreeNodeEx(formatString(key).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
		ImGui::Indent(10);
		return std::make_unique<PropertyMenuVisitor>(m_idResolution);
	}
	return nullptr;
}

void PropertyMenuVisitor::endChild(std::string const& key)
{
	ImGui::Unindent(10);
}

std::unique_ptr<IPropertyVisitor> PropertyMenuVisitor::collectionVisitor(std::string const& key, size_t& size, MetaData const& metaData) {
	if (ImGui::TreeNodeEx(formatString(key).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
		ImGui::Indent(10);
		if (ImGui::Button(formatString("+").c_str())) {
			++size;
		}

		return std::make_unique<PropertyMenuVisitor>(m_idResolution);
	}
	return nullptr;
}

void PropertyMenuVisitor::endCollection(std::string const& key)
{
	ImGui::Unindent(10);
}

void PropertyMenuVisitor::handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData)
{
	std::string name = key;
	if (pValue != nullptr) {
		GameObject* pGameObject = dynamic_cast<GameObject*>(pValue);
		if (pGameObject != nullptr) {
			name = pGameObject->getAlias();
		}
	}
	 

	if (ImGui::TreeNodeEx(formatString(name).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {

		ImGui::Indent(10);
		if (pValue == nullptr) {

			if (ImGui::BeginMenu(formatString("Create").c_str())) {

					
				factory.foreach_key([&factory, &pValue](std::string const& key) {
						if (ImGui::Button(std::format(">{}", key).c_str())) {
							factory.create(key, pValue);
						}
					}
				);

				ImGui::EndMenu();
			}

			ImGui::Text(std::format("No {} is attached", key).c_str());

			ImGui::Unindent(10);
			return;
		}

		IPropertyProvider* pPropertyProvider = dynamic_cast<IPropertyProvider*>(pValue);
		if (pPropertyProvider == nullptr) {
			return;
		}
		
		pPropertyProvider->properties(*this);
		
		ImGui::Unindent(10);
	}
}
