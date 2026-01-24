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
}
