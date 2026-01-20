#pragma once
#include "assert_define.h"
#include "key_enum.h"
#include "ngmath.h"



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

    static uint const MOUSE_BUTTON_STATES_TO_CAPTURE = 3u;
    static uint const KEY_BUTTON_STATES_TO_CAPTURE = 155u;//ImGuiKey_NamedKey_COUNT (TO DO): Eventually remove dependency on IMGUI?

    float m_mouseWheelScroll;
    Vector2 m_mousePos;
    Vector2 m_mouseDelta;

    ButtonState m_mouseButtonStates[MOUSE_BUTTON_STATES_TO_CAPTURE];
    ButtonState m_keyStates[KEY_BUTTON_STATES_TO_CAPTURE];
};