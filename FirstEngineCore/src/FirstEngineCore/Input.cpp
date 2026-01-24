#include "FirstEngineCore/Input.hpp"

namespace FirstEngine {
    //массив с нажатыми клавишами
    bool Input::m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
    bool Input::m_mouse_buttons_pressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1];
     
    //проверка
    bool Input::IsKeyPressed(const KeyCode key_code)
    {
        return m_keys_pressed[static_cast<size_t>(key_code)];
    }
    //массив заполняется нажатыми клавишами(true) 
    void Input::PressKey(const KeyCode key_code)
    {
        m_keys_pressed[static_cast<size_t>(key_code)] = true;
    }
    //если клавиша отжата 
    void Input::ReleaseKey(const KeyCode key_code)
    {
        m_keys_pressed[static_cast<size_t>(key_code)] = false;
    }
    //если мышка нажата 
    bool Input::IsMouseButtonPressed(const MouseButton mouse_button)
    {
        return m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)];
    }
    //нажатие мышки 
    void Input::PressMouseButton(const MouseButton mouse_button)
    {
        m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = true;
    }
    //мышку отпустили 
    void Input::ReleaseMouseButton(const MouseButton mouse_button)
    {
        m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = false;
    }
}
}
