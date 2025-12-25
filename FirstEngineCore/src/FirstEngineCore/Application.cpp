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