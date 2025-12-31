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
        
		//создаем окно 
		m_pWindow = std::make_unique<Window>(title, window_width, window_height);

		//---------------------коллбэки по событию---------------------------------------
		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event)
			{
				LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
		});

		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[&](EventWindowResize& event)
			{
				LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
				
		});
		//каждый раз когда приходит какой то ивент
		m_pWindow->set_event_callback(
			[&](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			}
		);
		//-----------------------------------------------------------------------------

		//игровой цикл , который будет рисовать каждый кадр игры
		while (true)
		{
			//для своего окна 
			m_pWindow->on_update();
			//для пользователя движка , для переиспользования 
			on_update();
		}

        return 0;
	}


}