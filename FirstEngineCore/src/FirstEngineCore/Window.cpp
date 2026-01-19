#include "FirstEngineCore/Window.hpp"
#include "FirstEngineCore/Log.hpp"
#include "FirstEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "FirstEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "FirstEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "FirstEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "FirstEngineCore/Camera.hpp"
#include "FirstEngineCore/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "FirstEngineCore/Modules/UIModule.hpp"

#include <GLFW/glfw3.h> 
#include <gl/GL.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

namespace FirstEngine {

	  //конструктор окна
	Window::Window(std::string title, const unsigned int width, const unsigned int height)
		:m_data({ std::move(title), width, height })
	{
		int resultCode = init();

	}

	//деструктор окна 
	Window::~Window() {
		shutdown();
	}

	//инициализация окна
	int Window::init() {
		LOG_INFO("Creating window `{0}` width size {1}x{2}", m_data.title, m_data.width, m_data.height);

		//ловим ошибки GLFW
		glfwSetErrorCallback([](int error_code, const char* description)
			{
				LOG_CRITICAL("GLFW error: {0}", description);
			});
		//инициализируем glfw 
		if (!glfwInit())
		{
			LOG_CRITICAL("Can't initialize GLFW!");
			return -1;
		}

		//создаем окно
		m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		//если окно не создалось 
		if (!m_pWindow)
		{
			LOG_CRITICAL("Can't create window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height)
				return -2;
		}

		//инициализируем GLFW
		if (!Renderer_OpenGL::init(m_pWindow))
		{
			LOG_CRITICAL("Failed to initialize OpenGL renderer");
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

		//---------------------------------работа с шейдерами--------------------------------------

		//получение кол-ва пикселей (размер окна  для ретина-мониторов) 
		glfwSetFramebufferSizeCallback(m_pWindow,
			[](GLFWwindow* pWindow, int width, int height)
			{
				Renderer_OpenGL::set_viewport(width, height);
			}
		);

  //при создании окна 
  UIModule::on_window_create(m_pWindow);

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

		Renderer_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);		//изменяем буфер цвета       
		Renderer_OpenGL::clear();       //рисуем  

		//----------------------------------------ОТРИСОВКА ВСЕГО OpenGL-------------------------------------------  
		//выбираем текущую шейдерную программу 	
		p_shader_program->bind();
		//матрица трансформации размера
		glm::mat4 scale_matrix(scale[0], 0,	       0,        0,
							   0,		 scale[1], 0,        0,
							   0,        0,        scale[2], 0,
							   0,        0,        0,        1);
		//поворот в радианах
		float rotate_in_radians = glm::radians(rotate);
		//матрица трансформации поворота
		glm::mat4 rotate_matrix(cos(rotate_in_radians),	sin(rotate_in_radians),	0, 0,
								-sin(rotate_in_radians),cos(rotate_in_radians),	0, 0,
								0,						0,						1, 0,
								0,						0,						0, 1);

		//матрица трансформации перемещения
		glm::mat4 translate_matrix(1,			0,			 0,			   0,
								   0,			1,			 0,			   0,
								   0,			0,			 1,			   0,
								   translate[0],translate[1],translate[2], 1);
		//получаем модел матрикс
		glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
		//передаем матрицу модели в шейдерную программу
		p_shader_program->setMatrix4("model_matrix", model_matrix);
		//================================камера==============================
		camera.set_position_rotation(glm::vec3(camera_position[0], camera_position[1], camera_position[2]),
		glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
		camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
		p_shader_program->setMatrix4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());
		//=====================================================================
		//----------------------------ОТРИСОВКА ВСЕГО OpenGL закончена--------------------------------------------
		 

		//----------------------------ОТРИСОВКА ИНТЕРФЕЙСА-----------------------------------
  //openGL рендер 
		Renderer_OpenGL::draw(*p_vao);
  //начало рисовки UI
  UIModule::on_ui_draw_begin();
		
//виджеты 
  bool show = true;          UIModule::ShowExampleAppDockSpace(&show);
		//ImGui::ShowDemoWindow();        //демо
		ImGui::Begin("Background Color Window");									//начало нового окна
		ImGui::ColorEdit4("Background Color", m_background_color);					//виджет изменения цвета
		ImGui::SliderFloat3("scale", scale, 0.f, 2.f);								//увеличение
		ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);							//поворот
		ImGui::SliderFloat3("translate", translate, -1.f, 1.f);						//перемещение
		ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);		//позиция камеры
		ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);			//поворот камеры
		ImGui::Checkbox("Perspective camera", &perspective_camera);					//перспектива                ImGui::End();                                               //закрытие окна
UIModule::on_ui_draw_end()
		//меняем front and back buffer
		glfwSwapBuffers(m_pWindow);
  //
		glfwPollEvents();
	}

	//закрытие окна
	void Window::shutdown() {
  //при закрытии окна 
  UIModule::on_window_close();
		//удаляем GLFW окно
		glfwDestroyWindow(m_pWindow);
		//удаляем GLFW контекст
		glfwTerminate();
	}
}