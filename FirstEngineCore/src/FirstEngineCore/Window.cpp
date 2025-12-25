#include "FirstEngineCore/Window.hpp"
#include "FirstEngineCore/Log.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace FirstEngine {

    static bool s_GLFW_initialized = false;

	Window::Window(std::string title, const unsigned int width, const unsigned int height) 
		:m_title(std::move(title))
		, m_width(width)
		, m_height(height)
	{
		int resultCode = init();
	}
	Window::~Window() {
		shutdown();
	}

	//функция будет вызываться каждый кадр 
	//каждый кадр , который будет прорисовываться , будет вызывать внутри себя эту функцию
	void Window::on_update() {
        //изменить цвет заливки буфера
        glClearColor(1, 0, 0, 0);

        /* Render here */
        //в каждый кадр заливаем этот буфер 
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(m_pWindow);

        /* Poll for and process events */
        glfwPollEvents();
	}

	//вспомогательная функция для инициализации в конструкции
	int Window::init() {
        LOG_INFO("Creating window `{0}` width size {1}x{2}", m_title, m_width, m_height);

        /* Initialize the library */
        //инициализация glfw 
        if (!s_GLFW_initialized) {
            if (!glfwInit())
            {
                LOG_CRITICAL("Can't initialize GLFW!")
                return -1;
            }
            s_GLFW_initialized = true;
        }
        

        //создаем окно
        m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        //если окно не создалось 
        if (!m_pWindow)
        {
            LOG_CRITICAL("Can't create window {0} width size {1}x{2}", m_title, m_width, m_height)
            glfwTerminate();
            return -2;
        }

        /* Make the window's context current */
        //создаем текущий контекст окна
        glfwMakeContextCurrent(m_pWindow);

        //загружаем функции openGL через GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Failed to initialize GLAD")
                return -3;
        }

        return 0;
        
	}
	//вспомогательная функция для уничтожения окна
	void Window::shutdown() {

        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
	}
}