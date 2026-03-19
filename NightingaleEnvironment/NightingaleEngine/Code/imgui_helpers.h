#pragma once
#include "ngmath.h"

class ImGuiHelpers {

public:
    static bool ColorPicker3(const char* label, Color& color, ImGuiColorEditFlags flags = 0);
    static bool ColorPicker4(const char* label, Color& color, ImGuiColorEditFlags flags = 0);

    static bool Vector3Input(const char* label, Vector3& vector, float vSpeed = 1.0f, float vMin = 0.0f, float vMax = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    
    static bool AxisAlignedBoxInput(const char* label, AxisAlignedBox& AxisAlignedBox, float vSpeed = 1.0f, float vMin = 0.0f, float vMax = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

};