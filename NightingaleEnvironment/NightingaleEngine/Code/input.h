#pragma once
#include "vector.h"

enum class MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2
};

//Keys that are used must be registed here
enum class Key {
    Tilda = ImGuiKey_GraveAccent,
    Enter = ImGuiKey_Enter,
    Shift = ImGuiKey_LeftShift,

    UpArrow = ImGuiKey_UpArrow,
    DownArrow = ImGuiKey_DownArrow,

    W = ImGuiKey_W,
    A = ImGuiKey_A,
    S = ImGuiKey_S,
    D = ImGuiKey_D,
    F = ImGuiKey_F,
    C = ImGuiKey_C,
    X = ImGuiKey_X,
    Z = ImGuiKey_Z

};

class Input {
    //for simplicity, input is currently retrieved from IMGUI state
public:
    void init();
    void captureInputState();
    void shutdown();

    
    static Vector2 const& MousePos() { assert(s_pInput != nullptr); return s_pInput->m_mousePos; }
    static Vector2 const& MouseDelta() { assert(s_pInput != nullptr); return s_pInput->m_mouseDelta; }
    static float MouseScroll() { assert(s_pInput != nullptr); return s_pInput->m_mouseWheelScroll;  }

    static bool MouseDown(MouseButton button) { assert(s_pInput != nullptr); return s_pInput->m_mouseButtonStates[ (uint) button].down; }
    static bool MouseClicked(MouseButton button) { assert(s_pInput != nullptr); return s_pInput->m_mouseButtonStates[(uint)button].clicked; }

    static bool KeyDown(Key key);
    static bool KeyClicked(Key key);
    
private:
    struct ButtonState {
        bool clicked;
        bool down;
    };


    static Input* s_pInput;

    static uint const MOUSE_BUTTON_STATES_TO_CAPTURE = 3;
    static uint const KEY_BUTTON_STATES_TO_CAPTURE = ImGuiKey_NamedKey_COUNT;

    float m_mouseWheelScroll;
    Vector2 m_mousePos;
    Vector2 m_mouseDelta;

    ButtonState m_mouseButtonStates[MOUSE_BUTTON_STATES_TO_CAPTURE];
    ButtonState m_keyStates[KEY_BUTTON_STATES_TO_CAPTURE];
};