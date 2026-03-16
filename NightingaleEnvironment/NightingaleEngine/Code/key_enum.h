#pragma once

//Mouse button enum
enum class MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2
};

//Keys that are used must be registed here
//Based on IMGUI Keys
enum class Key {
    Invalid = 0u,

    Tilda = 606u,//ImGuiKey_GraveAccent
    Enter = 525u,//ImGuiKey_Enter
    LControl = 527u,
    LShift = 528u,//ImGuiKey_LeftShift
    
    RightArrow = 513u,
    LeftArrow = 514u,
    UpArrow = 515u,
    DownArrow = 516u,

    A = 546u + 0u, //ImGuiKey_A
    B = 546u + 1u,
    C = 546u + 2u,
    D = 546u + 3u,
    E = 546u + 4u,
    F = 546u + 5u,
    G = 546u + 6u,
    H = 546u + 7u,
    I = 546u + 8u,
    J = 546u + 9u,
    K = 546u + 10u,
    L = 546u + 11u,
    M = 546u + 12u,
    N = 546u + 13u,
    O = 546u + 14u,
    P = 546u + 15u,
    Q = 546u + 16u,
    R = 546u + 17u,
    S = 546u + 18u,
    T = 546u + 19u,
    U = 546u + 20u,
    V = 546u + 21u,
    W = 546u + 22u,
    X = 546u + 23u,
    Y = 546u + 24u,
    Z = 546u + 25u,

};

static std::string KeyToString(Key key) {
    switch (key)
    {
    case Key::Tilda: return "Tilda";
    case Key::Enter: return "Enter";
    case Key::LControl: return "LControl";
    case Key::LShift: return "LShift";

    case Key::RightArrow: return "RightArrow";
    case Key::LeftArrow: return "LeftArrow";
    case Key::UpArrow: return "UpArrow";
    case Key::DownArrow: return "DownArrow";

    case Key::A: return "A";
    case Key::B: return "B";
    case Key::C: return "C";
    case Key::D: return "D";
    case Key::E: return "E";
    case Key::F: return "F";
    case Key::G: return "G";
    case Key::H: return "H";
    case Key::I: return "I";
    case Key::J: return "J";
    case Key::K: return "K";
    case Key::L: return "L";
    case Key::M: return "M";
    case Key::N: return "N";
    case Key::O: return "O";
    case Key::P: return "P";
    case Key::Q: return "Q";
    case Key::R: return "R";
    case Key::S: return "S";
    case Key::T: return "T";
    case Key::U: return "U";
    case Key::V: return "V";
    case Key::W: return "W";
    case Key::X: return "X";
    case Key::Y: return "Y";
    case Key::Z: return "Z";

    default: return "Unknown";
    }
}

static Key StringToKey(std::string const& str)
{
    if (str == "Tilda") return Key::Tilda;
    if (str == "Enter") return Key::Enter;
    if (str == "LControl") return Key::LControl;
    if (str == "LShift") return Key::LShift;

    if (str == "RightArrow") return Key::RightArrow;
    if (str == "LeftArrow") return Key::LeftArrow;
    if (str == "UpArrow") return Key::UpArrow;
    if (str == "DownArrow") return Key::DownArrow;

    if (str == "A") return Key::A;
    if (str == "B") return Key::B;
    if (str == "C") return Key::C;
    if (str == "D") return Key::D;
    if (str == "E") return Key::E;
    if (str == "F") return Key::F;
    if (str == "G") return Key::G;
    if (str == "H") return Key::H;
    if (str == "I") return Key::I;
    if (str == "J") return Key::J;
    if (str == "K") return Key::K;
    if (str == "L") return Key::L;
    if (str == "M") return Key::M;
    if (str == "N") return Key::N;
    if (str == "O") return Key::O;
    if (str == "P") return Key::P;
    if (str == "Q") return Key::Q;
    if (str == "R") return Key::R;
    if (str == "S") return Key::S;
    if (str == "T") return Key::T;
    if (str == "U") return Key::U;
    if (str == "V") return Key::V;
    if (str == "W") return Key::W;
    if (str == "X") return Key::X;
    if (str == "Y") return Key::Y;
    if (str == "Z") return Key::Z;

    return Key::Invalid;
}