#include "FirstEngineCore/Window.hpp"
#include "FirstEngineCore/Log.hpp"
#include "FirstEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "FirstEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "FirstEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "FirstEngineCore/Rendering/OpenGL/IndexBuffer.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h> 
#include <gl/GL.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace FirstEngine {

    static bool s_GLFW_initialized = false;
    
    //vertex точек и цвета 
    GLfloat positions_colors2[] = 
    {
       -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f
    };
    
    //индексы для index buffer
    GLuint indices[] = {
        0, 1, 2, 3, 2, 1
    };

    const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(color, 1.0);"
        "}";

    //указатель на шейдерную программу
    std::unique_ptr<ShaderProgram> p_shader_program;


    //указатели vertex буфер 
    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    //index buffer
    std::unique_ptr<IndexBuffer> p_index_buffer;
    //vertex array
    std::unique_ptr<VertexArray> p_vao;

    //конструктор окна
        Window::Window(std::string title, const unsigned int width, const unsigned int height) 
        :m_data({ std::move(title), width, height })
        {
                int resultCode = init();

        //подключение ImGui
        IMGUI_CHECKVERSION();       //проверяем версию

        ImGui::CreateContext();     //создаем контекст 

        ImGui_ImplOpenGL3_Init();   //opengl для imgui

        ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true); //инициализируем бэкенд imgui для связки с glfw для openGl(можно и vulkan)

        }

    //деструктор окна 
        Window::~Window() {
                shutdown();
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

        //---------------------работа с шейдерами--------------------------

        //получение кол-ва пикселей (размер окна  для ретина-мониторов) 
        glfwSetFramebufferSizeCallback(m_pWindow, 
            [](GLFWwindow* pWindow, int width, int height)
            {
                glViewport(0, 0, width, height);

            }
        );

        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled())
        {
            return false;
        };

        //передаем всю необходимую информацию о шейдерах в память видеокарты
//создаем окружение для 1 буфера 
BufferLayout buffer_layout_2vec3
{
   ShaderDataType::Float3,
   ShaderDataType::Float3
};

//vertex array для 1 буфера 
p_vao = std::make_unique<VertexArray>();

//vertex буфер сразу для точек и цвета 
p_positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors2, sizeof(positions_colors2), buffer_layout_2vec3);
//index buffer с несколькими буферами 
p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

//добавляем буфер в vertex array
p_vao->add_vertex_buffer(*p_positions_colors_vbo);
//устанавливаем index buffer в vertex array
p_vao->set_index_buffer(*p_index_buffer); 

        //---------------------------------------------------------------------


        return 0;

        }

    //изменение в окне , вся отрисовка тут
    void Window::on_update() {

        glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);           //изменяем буфер цвета       
        glClear(GL_COLOR_BUFFER_BIT);       //рисуем 


        ImGuiIO& io = ImGui::GetIO();       //хендл  ввода вывода

        io.DisplaySize.x = static_cast<float>(get_width());    //указать размер окна по горизонтали , виджеты должны совпадать с этим размером
        io.DisplaySize.y = static_cast<float>(get_height());   //указать размер окна по вертикали , виджеты должны совпадать с этим размером

        //создание фрейма где мы будем рисовать
        ImGui_ImplOpenGL3_NewFrame();   //кадр для openGL
        ImGui_ImplGlfw_NewFrame();      //кадр для glfw
        ImGui::NewFrame();              //кадр самого ImGui

        //виджеты 
        ImGui::ShowDemoWindow();        //демо

        //выбор цвета фона
        ImGui::Begin("Background Color Window");                    //начало нового окна
        ImGui::ColorEdit4("Background Color", m_background_color);  //виджет изменения цвета

//--------------------------работа с шейдерами-----------------------------    
   p_shader_program->bind();  //выбираем текущую шейдерную программу 
   p_vao->bind(); //текущая vertex array
   glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_vao->get_indices_count()), GL_UNSIGNED_INT, nullptr); //рисуем 

//------------------------------------------------------------------------
 ImGui::End();                                               //закрытие окна

        //рендер
        ImGui::Render();                                            //сохраняем данные для рендера
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());     //рисуем(можно поменять на vulkan) по данным рендера



        /* Swap front and back buffers */
        glfwSwapBuffers(m_pWindow);
        /* Poll for and process events */
        glfwPollEvents();
    }

        //закрытие окна
        void Window::shutdown() {

        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
        }
}
