#pragma once

namespace FirstEngine {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		//запрещаем конструкторы(потому что класс виртуальный)
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;
		//потому что не должно быть возможности копировать приложение , приложение должно быть одно

		//функция старта (ширина окна, высота окна, название окна)
		virtual int start(unsigned int window_width, unsigned int window_height, const char* title);

		//функция будет вызываться каждый кадр 
		//каждый кадр , который будет прорисовываться , будет вызывать внутри себя эту функцию
		virtual void on_update(){}
	};
}