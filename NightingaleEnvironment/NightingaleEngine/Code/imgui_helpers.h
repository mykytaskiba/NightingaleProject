#pragma once
#include "ngmath.h"

class ImGuiHelpers {

public:
    static bool ColorPicker3(const char* label, Color& color, ImGuiColorEditFlags flags = 0);
    static bool ColorPicker4(const char* label, Color& color, ImGuiColorEditFlags flags = 0);
};