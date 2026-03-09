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
