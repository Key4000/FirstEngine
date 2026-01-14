#pragma once

#include "FirstEngineCore/Event.hpp"

#include <string>
#include <functional>

struct GLFWwindow;

namespace FirstEngine {
	class Window
	{
	public:
		//call back , функция для возврата 
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		//запрещаем конструктор копирования и присваивания 
		Window(const Window&) = delete;
		//запрещаем конструктор перемещения 
		Window(Window&&) = delete;
		//запрещаем оператор присваивания и 
		Window& operator=(const Window&) = delete;
		//запрещаем оператор перемещения 
		Window& operator=(Window&&) = delete;
		//потому что не должно быть возможности копировать приложение , приложение должно быть одно


		//функция будет вызываться каждый кадр 
		//каждый кадр , который будет прорисовываться , будет вызывать внутри себя эту функцию
		void on_update();

		unsigned int get_width() const { return m_data.width; }
		unsigned int get_height() const { return m_data.height; }

		//функция для создания колбэка
		void set_event_callback(const EventCallbackFn& callback) {
			m_data.eventCallbackFn = callback;
		}

	private:
		//данные окна
		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallbackFn eventCallbackFn;
		};


		//вспомогательная функция для инициализации в конструкции
		int init();
		//вспомогательная функция для уничтожения окна
		void shutdown();

		//хендл окна
		GLFWwindow* m_pWindow = nullptr;

		//сама переменная с данными
		WindowData m_data;

		float m_background_color[4] = { 0.f, 0.f, 0.f, 0.f }; //для хранения цвета фона

	};
}