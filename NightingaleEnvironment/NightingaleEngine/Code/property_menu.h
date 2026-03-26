#pragma once
#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "property_visitor.h"
#include "ngmath.h"
#include "imgui_helpers.h"

struct PropertyMenuVisitor : public IPropertyVisitor {
	
	template<typename TValue>
	void visit(std::string const& key, TValue& value, MetaData const& metaData) {
		if (metaData.m_bReadOnly) ImGui::BeginDisabled();
		menu(key, value, metaData);
		if (metaData.m_bReadOnly) ImGui::EndDisabled();
	}

	/*
	template<typename TValue>
	void visit(std::string const& key, TValue*& value, MetaData const& metaData) {
	}*/

	template<typename TValue>
	void menu(std::string const& key, TValue& value, MetaData const& metaData) {
		ImGui::Text("Property editor not defined: %s", key.c_str());
	}

	//NESTED PROPERTIES
	template<typename TValue>
	requires HasProperties<TValue>
	void menu(std::string const& key, TValue& value, MetaData const& metaData) {
		ImGui::Separator();
		if (ImGui::TreeNodeEx(key.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			value.properties(*this); //no reason for child visitor since this visitor does not hold data
		}
	}

	template<>
	void menu(std::string const& key, std::string& value, MetaData const& metaData) {
		ImGui::InputText(key.c_str(), &value);
	}

	template<>
	void menu(std::string const& key, int& value, MetaData const& metaData) {
		ImGui::InputInt(key.c_str(), &value);
	}

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

	}


	//DEFINE OVERRIDE FUNCTIONS
#define X(TType) \
	void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	void operator()(std::string const& key, std::vector<TType>& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	//END																									

	PROPERTY_TYPES
#undef X
};