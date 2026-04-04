#include "pch.h"
#include "property_menu.h"
#include "property_provider.h"
#include "game_object.h"

std::string PropertyMenuVisitor::formatStringForImGUI(std::string const& key)
{
	if (key.empty()) {
		return "";
	}

	std::string result;
	result.reserve(key.length());

	bool nextUpper = true;
	for (char c : key) {
		if (c == '_') {
			result += ' ';
			nextUpper = true;
		}
		else {
			if (nextUpper) {
				result += (char)std::toupper((unsigned char)c);
				nextUpper = false;
			}
			else {
				result += c;
			}
		}
	}
	return result;
}

std::unique_ptr<IPropertyVisitor> PropertyMenuVisitor::childVisitor(std::string const& key) {

	ImGui::PushID(key.c_str());
	if (ImGui::TreeNodeEx(formatStringForImGUI(key).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
		ImGui::Indent(10);
		return std::make_unique<PropertyMenuVisitor>();
	}
	ImGui::PopID();
	return nullptr;
}

void PropertyMenuVisitor::endChild(std::string const& key)
{
	ImGui::Unindent(10);
	ImGui::PopID();
}

std::unique_ptr<IPropertyVisitor> PropertyMenuVisitor::collectionVisitor(std::string const& key, size_t& size) {
	ImGui::PushID(key.c_str());
	if (ImGui::TreeNodeEx(formatStringForImGUI(key).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
		ImGui::Indent(10);
		if (ImGui::Button(formatStringForImGUI("+").c_str())) {
			++size;
		}

		return std::make_unique<PropertyMenuVisitor>();
	}
	ImGui::PopID();
	return nullptr;
}

void PropertyMenuVisitor::endCollection(std::string const& key)
{
	ImGui::Unindent(10);
	ImGui::PopID();
}

void PropertyMenuVisitor::handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory)
{
	std::string name = key;
	if (pValue != nullptr) {
		GameObject* pGameObject = dynamic_cast<GameObject*>(pValue);
		if (pGameObject != nullptr) {
			name = pGameObject->getAlias();
		}
	}
	 
	ImGui::PushID(key.c_str());
	if (ImGui::TreeNodeEx(formatStringForImGUI(name).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {

		ImGui::Indent(10);
		if (pValue == nullptr) {

			if (ImGui::BeginMenu(formatStringForImGUI("Create").c_str())) {


				factory.foreach_key([&factory, &pValue](std::string const& key) {
					if (ImGui::Button(std::format(">{}", key).c_str())) {
						factory.create(key, pValue);
					}
					}
				);

				ImGui::EndMenu();
			}

			ImGui::Text(std::format("No {} is attached", key).c_str());

		}
		else {
			IPropertyProvider* pPropertyProvider = dynamic_cast<IPropertyProvider*>(pValue);
			if (pPropertyProvider != nullptr) {
				pPropertyProvider->properties(*this);
			}
		}

	}
	ImGui::Unindent(10);
	ImGui::PopID();
}
