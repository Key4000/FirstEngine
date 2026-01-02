#include "FirstEngineCore/Window.hpp"
#include "FirstEngineCore/Log.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace FirstEngine {

    static bool s_GLFW_initialized = false;

	Window::Window(std::string title, const unsigned int width, const unsigned int height) 
        :m_data({ std::move(title), width, height })
	{
		int resultCode = init();
	}
	Window::~Window() {
		shutdown();
	}

	//наша фукция , для изменения в каждом кадре
	void Window::on_update() {
        //изменяем буфер цвета
        glClearColor(1, 0, 0, 0);

        //рисуем 
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(m_pWindow);

        /* Poll for and process events */
        glfwPollEvents();
	}

	//инициализация в нашем классе 
	int Window::init() {
        LOG_INFO("Creating window `{0}` width size {1}x{2}", m_data.title, m_data.width, m_data.height);

        /* Initialize the library */
        //инициализируем glfw 
        if (!s_GLFW_initialized) {
            if (!glfwInit())
            {
                LOG_CRITICAL("Can't initialize GLFW!")
                return -1;
            }
            s_GLFW_initialized = true;
        }
        

        //создаем окно
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        //если окно не создалось 
        if (!m_pWindow)
        {
            LOG_CRITICAL("Can't create window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height)
            glfwTerminate();
            return -2;
        }

        /* Make the window's context current */
        //создаем контекст окна
        glfwMakeContextCurrent(m_pWindow);

        //загружаем функции openGL с помощью GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Failed to initialize GLAD")
                return -3;
        }
        //оставляем ссылку на свои данные в хендле
        glfwSetWindowUserPointer(m_pWindow, &m_data);

        //------------------------ловим колбэки--------------------------------------------------

        //ловим колбэк изменения размера окна
        glfwSetWindowSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                //получаем данные из хендла
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

                data.width = width;
                data.height = height;

                //пишем данные , чтобы отправить в наш ивент 
                EventWindowResize event(width, height);
                //вызываем наш колбэк
                data.eventCallbackFn(event);
                
            }
        );

        //ловим колбэк изменения позиции курсора 
        glfwSetCursorPosCallback(m_pWindow,
            [](GLFWwindow* pWindow, double x, double y) {

                //получаем данные из хендла
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));      

                //пишем данные , чтобы отправить в наш ивент 
                EventMouseMoved event(x, y);
                //вызываем наш колбэк
                data.eventCallbackFn(event);
            }
        );

        //--------------------------------------------------------------------------------------------------

        return 0;
        
	}
	//закрытие окна
	void Window::shutdown() {

        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
	}
}