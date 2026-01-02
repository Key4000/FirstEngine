#include "FirstEngineCore/Window.hpp"
#include "FirstEngineCore/Log.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace FirstEngine {

    static bool s_GLFW_initialized = false;

    //конструктор окна
	Window::Window(std::string title, const unsigned int width, const unsigned int height) 
        :m_data({ std::move(title), width, height })
	{
		int resultCode = init();

        //подключение ImGui
        //проверяем версию
        IMGUI_CHECKVERSION();
        //создаем контекст 
        ImGui::CreateContext();
        //opengl для imgui
        ImGui_ImplOpenGL3_Init();

	}

    //деструктор окна 
	Window::~Window() {
		shutdown();
	}

	//изменение в окне , вся отрисовка тут
	void Window::on_update() {
       
        glClearColor(0, 0, 0, 0);  //изменяем буфер цвета       
        glClear(GL_COLOR_BUFFER_BIT); //рисуем 

        ImGuiIO& io = ImGui::GetIO(); //хендл  ввода вывода
        
        io.DisplaySize.x = static_cast<float>(get_width());    //указать размер окна по горизонтали , виджеты должны совпадать с этим размером
        io.DisplaySize.y = static_cast<float>(get_height());   //указать размер окна по вертикали , виджеты должны совпадать с этим размером
        
        //создание фрейма где мы будем рисовать
        ImGui_ImplOpenGL3_NewFrame();   //начинаем новый фрейм 
        ImGui::NewFrame();  //создаем новый фрейм самого ImGui
        
        //виджеты 
        ImGui::ShowDemoWindow();   

        //рендер
        ImGui::Render();    //создаем данные для рендера в GetDrawData
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());     //рисуем(можно поменять на vulkan) по данным из GetDrawData



        /* Swap front and back buffers */
        glfwSwapBuffers(m_pWindow);
        /* Poll for and process events */
        glfwPollEvents();
	}

	//инициализация окна
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

        //ловим колбэк закрытия окна
        glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow) {
            //получаем данные из хендла
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            //пишем данные , чтобы отправить в наш ивент 
            EventWindowClose event;
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