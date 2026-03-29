#pragma once
#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "property_visitor.h"
#include "ngmath.h"
#include "imgui_helpers.h"
#include "property_metadata.h"

struct PropertyMenuVisitor : public IPropertyVisitor {

	void operator()(std::string const& key, int& value, MetaData const& metaData = {}) override {
		ImGui::InputInt(key.c_str(), &value);
	}
	void operator()(std::string const& key, float& value, MetaData const& metaData = {}) override {
		ImGui::InputFloat(key.c_str(), &value);
	}
	void operator()(std::string const& key, unsigned int& value, MetaData const& metaData = {}) override {
		unsigned int step = 1;
		ImGui::InputScalar(key.c_str(), ImGuiDataType_U32, &value, &step);
	}
	void operator()(std::string const& key, std::string& value, MetaData const& metaData = {}) override {
		ImGui::InputText(key.c_str(), &value);
	}

protected:
	bool enterScope(std::string const& key, MetaData const& metaData) override {
		ImGui::Separator();
		if (ImGui::TreeNodeEx(key.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			return true;
		}
	}
	void leaveScope(MetaData const& metaData) override {
	}

	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData) override {

	}

	/*
	template<typename TValue>
	void visit(std::string const& key, TValue& value, MetaData const& metaData) {
		if (metaData.m_bReadOnly) ImGui::BeginDisabled();
		menu(key, value, metaData);
		if (metaData.m_bReadOnly) ImGui::EndDisabled();
	}*/

	/*
	template<typename TValue>
	void visit(std::string const& key, TValue*& value, MetaData const& metaData) {
	}*/

	/*
	template<>
	void menu(std::string const& key, uint& value, MetaData const& metaData) {
		
		int tempInt{ (int)value };
		menu<int>(key, tempInt, metaData);
		if (tempInt < 0) {
			tempInt = 0;
		}
		value = tempInt;
	}

	template<>
	void menu(std::string const& key, float& value, MetaData const& metaData) {
		ImGui::InputFloat(key.c_str(), &value);
	}

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