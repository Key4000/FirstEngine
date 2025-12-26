#pragma once

#include <string>

struct GLFWwindow;

namespace FirstEngine {
	class Window
	{
	public:
		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		//запрещаем конструктор создания и копирования
		Window(const Window&) = delete;
  //запрещаем конструктор перемещения 
		Window(Window&&) = delete;
  //запрещаем оператор присваивания 
		Window& operator=(const Window&) = delete;
  //запрещаем оператор перемещения 
		Window& operator=(Window&&) = delete;
		//потому что не должно быть возможности копировать приложение , приложение должно быть одно

		
		//функция будет вызываться каждый кадр 
		//каждый кадр , который будет прорисовываться , будет вызывать внутри себя эту функцию
		void on_update();

		unsigned int get_width() const { return m_width; }
		unsigned int get_height() const { return m_height; }

	private: 
		//вспомогательная функция для инициализации в конструкции
		int init();
		//вспомогательная функция для уничтожения окна
		void shutdown();

		//хендл окна
		GLFWwindow* m_pWindow;

		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;
	};
}