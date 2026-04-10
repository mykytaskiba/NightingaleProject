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

    constexpr bool operator==(Color const& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    constexpr bool operator!=(Color const& other) const {
        return !(*this == other);
    }


private:



};

void to_json(nlohmann::json& json, const Color& color);
void from_json(nlohmann::json const& json, Color& color);