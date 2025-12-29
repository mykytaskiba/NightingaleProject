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
    Tilda = 606u,//ImGuiKey_GraveAccent
    Enter = 525u,//ImGuiKey_Enter
    Shift = 528u,//ImGuiKey_LeftShift

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