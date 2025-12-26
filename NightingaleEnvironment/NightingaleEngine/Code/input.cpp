#include "pch.h"
#include "input.h"

Input* Input::s_pInput = nullptr;

void Input::init()
{
    s_pInput = this;
}

void Input::captureInputState()
{
    ImGuiIO& io = ImGui::GetIO();

    //Store input state
    m_mousePos = Vector2(io.MousePos.x,io.MousePos.y);
    m_mouseDelta = Vector2(io.MouseDelta.x, io.MouseDelta.y);

    m_mouseWheelScroll = io.MouseWheel;
    
    for (int i = 0; i < MOUSE_BUTTON_STATES_TO_CAPTURE; i++) {
        m_mouseButtonStates[i].clicked = io.MouseClicked[i];
        m_mouseButtonStates[i].down = io.MouseDown[i];
    }
    for (int i = 0; i < KEY_BUTTON_STATES_TO_CAPTURE; i++) {
        bool keyState = io.KeysData[i].Down;
        bool prevKeyState = m_keyStates[i].down;

        m_keyStates[i].clicked = (keyState && !prevKeyState);
        m_keyStates[i].down = keyState;
    }
    //io.KeysData[ImGuiKey_NamedKey_BEGIN];

    
}

void Input::shutdown()
{
}

bool Input::KeyDown(Key key)
{
    assert(s_pInput != nullptr);
    return s_pInput->m_keyStates[(uint)key - ImGuiKey_NamedKey_BEGIN].down;
}

bool Input::KeyClicked(Key key)
{
    assert(s_pInput != nullptr);
    return s_pInput->m_keyStates[(uint)key - ImGuiKey_NamedKey_BEGIN].clicked;
}
