#pragma once
#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "properties.h"
#include "ngmath.h"
#include "imgui_helpers.h"

struct PropertyMenuVisitor {
	
	template<typename TValue, typename... TMeta >
	void operator()(std::string const& key, TValue& value, TMeta&&... meta) {
		if (Meta::has<Meta::ReadOnly>(meta...)) ImGui::BeginDisabled();
		menu(key, value, std::forward<TMeta>(meta)...);
		if (Meta::has<Meta::ReadOnly>(meta...)) ImGui::EndDisabled();
	}


	template<typename TValue, typename... TMeta >
	void menu(std::string const& key, TValue& value, TMeta&&... meta) {
		ImGui::Text("Property editor not defined: %s", key.c_str());
	}

	template<typename TValue, typename... TMeta >
	requires HasProperties<TValue>
	void menu(std::string const& key, TValue& value, TMeta&&... meta) {
		if (ImGui::TreeNodeEx(key.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			value.properties(*this); //no reason for child visitor since this visitor does not hold data
		}
	}

	template<typename... TMeta >
	void menu(std::string const& key, std::string& value, TMeta&&... meta) {
		ImGui::InputText(key.c_str(), &value);
	}

	template<typename... TMeta >
	void menu(std::string const& key, Vector3& value, TMeta&&... meta) {
		ImGuiHelpers::Vector3Input(key.c_str(), value);
	}

	template<typename... TMeta >
	void menu(std::string const& key, Quaternion& value, TMeta&&... meta) {
		ImGuiHelpers::QuaternionInput(key.c_str(), value);
	}

};