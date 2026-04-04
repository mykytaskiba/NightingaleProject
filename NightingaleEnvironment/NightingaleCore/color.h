#pragma once
#include "json.hpp"

using ColorChannel = float;

class Color {
    
public:
    ColorChannel r;
    ColorChannel g;
    ColorChannel b;
    ColorChannel a;

    constexpr Color(ColorChannel r_, ColorChannel g_, ColorChannel b_, ColorChannel a_) :
        r(r_), g(g_), b(b_), a(a_) {}

private:



};

void to_json(nlohmann::json& json, const Color& color);
void from_json(nlohmann::json const& json, Color& color);