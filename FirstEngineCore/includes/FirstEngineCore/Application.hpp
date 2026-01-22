#pragma once

#include "FirstEngineCore/Event.hpp"
#include "FirstEngineCore/Camera.hpp"

#include <memory>

namespace FirstEngine {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int start(unsigned int window_width, unsigned int window_height, const char* title);

		virtual void on_update() {}

		//кастомный ui, для задания его в Editor
		virtual void on_ui_draw() {}

		//камера
		float camera_position[3] = { 0.f, 0.f, 1.f };
		float camera_rotation[3] = { 0.f, 0.f, 0.f };
		bool perspective_camera = true;

		//конструктор камеры с нач значением 
		Camera camera{ glm::vec3(-5, 0, 0) };

	private:
		//ссылка на окно 
		std::unique_ptr<class Window> m_pWindow;

		//класс обработчика событий 
		EventDispatcher m_event_dispatcher;
		bool m_bCloseWindow = false;
	};
}
