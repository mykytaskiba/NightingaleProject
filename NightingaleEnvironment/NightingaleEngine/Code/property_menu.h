#pragma once
#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "property_visitor.h"
#include "ngmath.h"
#include "imgui_helpers.h"
#include "property_metadata.h"


struct PropertyMenuVisitor : public IPropertyVisitor {


	template<typename TValue>
	void menuImpl(std::string const& key, TValue& value) {
		if (meta().isReadOnly()) ImGui::BeginDisabled();

		bool bChanged = false;
		if constexpr (std::is_same_v < TValue, bool>) {
			bChanged = ImGui::Checkbox(formatStringForImGUI(key).c_str(), &value);
		}
		else if constexpr (std::is_same_v < TValue, int>) {
			bChanged = ImGui::InputInt(formatStringForImGUI(key).c_str(), &value);
		}
		else if constexpr (std::is_same_v < TValue, float>) {
			if (meta().isReadOnly()) {
				ImGui::InputFloat(formatStringForImGUI(key).c_str(), &value, 0.01f, 1.0f, "%.10f");
			}
			else {
				bChanged = ImGui::InputFloat(formatStringForImGUI(key).c_str(), &value);
			}
		}
		else if constexpr (std::is_same_v < TValue, unsigned int>) {
			unsigned int step = 1;
			bChanged = ImGui::InputScalar(formatStringForImGUI(key).c_str(), ImGuiDataType_U32, &value, &step);
		}
		else if constexpr (std::is_same_v < TValue, std::string>) {
			bChanged = ImGui::InputText(formatStringForImGUI(key).c_str(), &value);
		}
		else if constexpr (std::is_same_v < TValue, Color>) {
			if (ImGui::CollapsingHeader(formatStringForImGUI(key).c_str())) {
				if (meta().isFlag(MetaFlags::ColorOnlyRGB)) {
					bChanged = ImGuiHelpers::ColorPicker3("", value);
				}
				else {
					bChanged = ImGuiHelpers::ColorPicker4("", value);
				}
			}
		}
		else if constexpr (std::is_same_v < TValue, Vector2 >) {

		}
		else if constexpr (std::is_same_v < TValue, Vector3 >) {
			bChanged = ImGuiHelpers::Vector3Input(formatStringForImGUI(key).c_str(), value);
		}
		else if constexpr (std::is_same_v < TValue, Vector4 >) {

		}


		if (meta().isReadOnly()) ImGui::EndDisabled();

		if (bChanged) meta().onChangeCallback();

	}

	void operator()(std::string const& key, bool& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, int& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, float& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, unsigned int& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, std::string& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, Color& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, Vector2& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, Vector3& value) override { menuImpl(key, value); }
	void operator()(std::string const& key, Vector4& value) override { menuImpl(key, value); }


protected:

	std::string formatStringForImGUI(std::string const& key);

	std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key) override;
	void endChild(std::string const& key) override;

	std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, size_t& size) override;
	void endCollection(std::string const& key) override;

	void enumerateCollectionItem(uint id) override;
	void endCollectionItem() override;

	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory) override;

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