#pragma once

struct GLFWwindow;

namespace FirstEngine {

    class UIModule
    {
    public:
        //при создании окна 
        static void on_window_create(GLFWwindow* pWindow);
        //при закрытии окна 
        static void on_window_close();
        //при начале отрисовки ui
        static void on_ui_draw_begin();
        //конец отрисовки ui
        static void on_ui_draw_end();
        //test
        static void ShowExampleAppDockSpace(bool* p_open);
    };
}
