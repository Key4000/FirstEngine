#include "FirstEngineCore/Application.hpp"
#include "FirstEngineCore/Log.hpp"
#include "FirstEngineCore/Window.hpp"

#include <iostream>

namespace FirstEngine {

    Application::Application() {
        LOG_INFO("Starting Application ");
    }
    Application::~Application() {
        LOG_INFO("Close Application ");
    }
    int Application::start(unsigned int window_width, unsigned int window_height, const char* title) {
        
        //хендл окна
        m_pWindow = std::make_unique<Window>(title, window_width, window_height);

        //---------------------слушатели событий---------------------------------------
        //слушатель на движение мыши
        m_event_dispatcher.add_event_listener<EventMouseMoved>(
            [](EventMouseMoved& event)
            {
                LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
        });
        //слушатель изменения окна 
        m_event_dispatcher.add_event_listener<EventWindowResize>(
            [](EventWindowResize& event)
            {
                LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
                
        });
        //слушатель на закрытие окна 
        m_event_dispatcher.add_event_listener<EventWindowClose>([&](EventWindowClose& event)
            {
                LOG_INFO("[WindowClose]");
                m_bCloseWindow = true;
            });
        //основной слушатель , который принимает все события 
        m_pWindow->set_event_callback(
            [&](BaseEvent& event)
            {
                m_event_dispatcher.dispatch(event);
            }
        );
        //-----------------------------------------------------------------------------

        //пока окно не закрыто цикл рендера работает
        while (!m_bCloseWindow)
        {
            m_pWindow->on_update();
            on_update();
        }
        //автоматически вызываем деструктор окна 
        m_pWindow = nullptr;

//тестируем кодировку


        return 0;
    }


}