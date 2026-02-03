#pragma once


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