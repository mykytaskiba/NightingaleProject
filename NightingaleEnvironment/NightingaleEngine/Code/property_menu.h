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

	template<typename TValue>
	void menuImpl(std::string const& key, TValue& value) {
		if (meta().isReadOnly()) ImGui::BeginDisabled();

		bool bChanged = false;
		if constexpr (std::is_same_v < TValue, bool>) {
			bChanged = ImGui::Checkbox(formatString(key).c_str(), &value);
		}
		else if constexpr (std::is_same_v < TValue, int>) {
			bChanged = ImGui::InputInt(formatString(key).c_str(), &value);
		}
		else if constexpr (std::is_same_v < TValue, float>) {
			bChanged = ImGui::InputFloat(formatString(key).c_str(), &value);
		}
		else if constexpr (std::is_same_v < TValue, unsigned int>) {
			unsigned int step = 1;
			bChanged = ImGui::InputScalar(formatString(key).c_str(), ImGuiDataType_U32, &value, &step);
		}
		else if constexpr (std::is_same_v < TValue, std::string>) {
			bChanged = ImGui::InputText(formatString(key).c_str(), &value);
		}


		if (meta().isReadOnly()) ImGui::EndDisabled();

		if (bChanged) meta().onChangeCallback();

	}

	void operator()(std::string const& key, bool& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, int& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, float& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, unsigned int& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, std::string& value) override { menuImpl(key, value); }


protected:

	std::string formatString(std::string const& key) {
		return std::format("{}##{}", key.c_str(), m_idResolution++);
	}

	std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key) override;
	void endChild(std::string const& key) override;

	std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, size_t& size) override;
	void endCollection(std::string const& key) override;

	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory) override;

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

class PropertyMenu {
public:

	template <typename TPropertyObject>
	requires HasProperties<TPropertyObject>
	static void render_update(TPropertyObject& propertyProvider) {
		PropertyMenuVisitor menuVisitor{};
		propertyProvider.properties(menuVisitor);
	}
};