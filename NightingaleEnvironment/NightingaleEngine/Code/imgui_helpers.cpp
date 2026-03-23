#include "pch.h"
#include "imgui_helpers.h"


bool ImGuiHelpers::ColorPicker3(const char* label, Color& color, ImGuiColorEditFlags flags)
{
    float colorArr[3] = { color.r, color.g, color.b };
    bool bChanged = ImGui::ColorPicker3(label, colorArr, flags);
    color.r = colorArr[0];
    color.g = colorArr[1];
    color.b = colorArr[2];
    return bChanged;
}

bool ImGuiHelpers::ColorPicker4(const char* label, Color& color, ImGuiColorEditFlags flags)
{
    float colorArr[4] = { color.r, color.g, color.b, color.a };
    bool bChanged = ImGui::ColorPicker4(label, colorArr, flags);
    color.r = colorArr[0];
    color.g = colorArr[1];
    color.b = colorArr[2];
    color.a = colorArr[3];
    return bChanged;
}

bool ImGuiHelpers::Vector3Input(const char* label, Vector3& vector, float vSpeed, float vMin, float vMax, const char* format, ImGuiSliderFlags flags)
{
    return ImGui::DragFloat3(label, &vector[0], vSpeed, vMin, vMax, format, flags);
}

bool ImGuiHelpers::QuaternionInput(const char* label, Quaternion& quat, float vSpeed, float vMin, float vMax, const char* format, ImGuiSliderFlags flags)
{
    float quatArr[4]{ quat.x,quat.y,quat.z,quat.w };
    if (ImGui::DragFloat4(label, &quatArr[0], vSpeed, vMin, vMax, format, flags)) {
        quat.x = quatArr[0];
        quat.y = quatArr[1];
        quat.z = quatArr[2];
        quat.w = quatArr[3];
        return true;
    }
    return false;
}

bool ImGuiHelpers::AxisAlignedBoxInput(const char* label, AxisAlignedBox& AxisAlignedBox, float vSpeed, float vMin, float vMax, const char* format, ImGuiSliderFlags flags)
{
    Vector3 offset = AxisAlignedBox.center();
    std::string offsetLabel{ std::string{label} + " offset" };

    Vector3 extents = AxisAlignedBox.extents();
    std::string extendsLabel{ std::string{label} + " extents" };

    bool bChanged{ false };
    bChanged |= Vector3Input(offsetLabel.c_str(), offset, vSpeed, vMin, vMax, format, flags);
    bChanged |= Vector3Input(extendsLabel.c_str(), extents, vSpeed, vMin, vMax, format, flags);

    AxisAlignedBox.setPositionExtents(offset, extents);

    return bChanged;
}