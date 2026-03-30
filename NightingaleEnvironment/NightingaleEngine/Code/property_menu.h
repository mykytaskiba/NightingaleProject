#pragma once
#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "property_visitor.h"
#include "ngmath.h"
#include "imgui_helpers.h"
#include "property_metadata.h"

struct PropertyMenuVisitor : public IPropertyVisitor {

	uint m_idResolution{ 0u };
	PropertyMenuVisitor(uint idResolution) : m_idResolution(idResolution) {}
	PropertyMenuVisitor() : m_idResolution(0u) {}

	void operator()(std::string const& key, int& value, MetaData const& metaData = {}) override {
		if (metaData.m_bReadOnly) ImGui::BeginDisabled();
		ImGui::InputInt(formatString(key).c_str(), &value);
		if (metaData.m_bReadOnly) ImGui::EndDisabled();
	}
	void operator()(std::string const& key, float& value, MetaData const& metaData = {}) override {
		if (metaData.m_bReadOnly) ImGui::BeginDisabled();
		ImGui::InputFloat(formatString(key).c_str(), &value);
		if (metaData.m_bReadOnly) ImGui::EndDisabled();
	}
	void operator()(std::string const& key, unsigned int& value, MetaData const& metaData = {}) override {
		if (metaData.m_bReadOnly) ImGui::BeginDisabled();
		unsigned int step = 1;
		ImGui::InputScalar(formatString(key).c_str(), ImGuiDataType_U32, &value, &step);
		if (metaData.m_bReadOnly) ImGui::EndDisabled();
	}
	void operator()(std::string const& key, std::string& value, MetaData const& metaData = {}) override {
		if (metaData.m_bReadOnly) ImGui::BeginDisabled();
		ImGui::InputText(formatString(key).c_str(), &value);
		if (metaData.m_bReadOnly) ImGui::EndDisabled();
	}

protected:

	std::string formatString(std::string const& key) {
		return std::format("{}##{}", key.c_str(), m_idResolution++);
	}

	std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key, MetaData const& metaData) override;
	void endChild(std::string const& key) override;

	std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, size_t& size, MetaData const& metaData) override;
	void endCollection(std::string const& key) override;

	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData) override;

	/*


	template<>
	void menu(std::string const& key, Vector3& value, MetaData const& metaData) {
		ImGuiHelpers::Vector3Input(key.c_str(), value);
	}

	template<>
	void menu(std::string const& key, Quaternion& value, MetaData const& metaData) {
		ImGuiHelpers::QuaternionInput(key.c_str(), value);
	}

	template<>
	void menu(std::string const& key, AxisAlignedBox& value, MetaData const& metaData) {

		if (metaData.m_bReadOnly) ImGui::EndDisabled(); //allow node to be toggled
		if (ImGui::TreeNodeEx(key.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			if (metaData.m_bReadOnly) ImGui::BeginDisabled();
			ImGuiHelpers::AxisAlignedBoxInput(key.c_str(), value);
			if (metaData.m_bReadOnly) ImGui::EndDisabled();
		}
		if (metaData.m_bReadOnly) ImGui::BeginDisabled();
	}

	template<typename TValue>
	void menu(std::string const& key, std::vector<TValue>& vector, MetaData const& metaData) {

		if (ImGui::TreeNodeEx(key.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {

			std::string plus = std::string{ "+##" } + key;
			if (ImGui::Button(plus.c_str())) {
				vector.push_back(TValue{});
			}

			uint idx{ 0u };
			for (auto& value : vector) {
				std::string idxStr = "[" + std::to_string(idx) + "]";
				menu(idxStr, value, metaData);
				++idx;
			}
		}

	}*/


};