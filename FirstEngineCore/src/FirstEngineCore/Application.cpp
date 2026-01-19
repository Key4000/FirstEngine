#include "FirstEngineCore/Application.hpp"
#include "FirstEngineCore/Log.hpp"
#include "FirstEngineCore/Window.hpp"
#include "FirstEngineCore/Event.hpp"

#include <iostream>

namespace FirstEngine {

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
		R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_color;
           uniform mat4 model_matrix;  
           uniform mat4 view_projection_matrix; 
           out vec3 color;
           void main() {
              color = vertex_color;
              gl_Position = view_projection_matrix *  model_matrix * vec4(vertex_position, 1.0);
           }
        )";


	const char* fragment_shader =
		R"(#version 460
           in vec3 color;
           out vec4 frag_color;
           void main() {
              frag_color = vec4(color, 1.0);
           }
        )";

	//указатель на шейдерную программу
	std::unique_ptr<ShaderProgram> p_shader_program;


	//указатели vertex буфер 
	std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
	//index buffer
	std::unique_ptr<IndexBuffer> p_index_buffer;
	//vertex array
	std::unique_ptr<VertexArray> p_vao;

	float scale[3] = { 1.f, 1.f, 1.f };
	float rotate = 0.f;
	float translate[3] = { 0.f, 0.f, 0.f };
	//камера 
	float camera_position[3] = { 0.f, 0.f, 1.f };
	float camera_rotation[3] = { 0.f, 0.f, 0.f };
	bool perspective_camera = false;
	Camera camera;

//

	Application::Application() {
		LOG_INFO("Starting Application ");
	}
	Application::~Application() {
		LOG_INFO("Close Application ");
	}
	int Application::start(unsigned int window_width, unsigned int window_height, const char* title) {

		//хендл окна
		m_pWindow = std::make_unique<Window>(title, window_width, window_height);

		//---------------------слушатели событий---------------------------------------
		//слушатель на движение мыши
		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event)
			{
				//LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
			});
		//слушатель изменения окна 
		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event)
			{
				LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);

			});
		//слушатель на закрытие окна 
		m_event_dispatcher.add_event_listener<EventWindowClose>([&](EventWindowClose& event)
			{
				LOG_INFO("[WindowClose]");
				m_bCloseWindow = true;
			});

		//вызывается каждый раз когда приходит какой то ивент от хендла окна
		m_pWindow->set_event_callback(
			[&](BaseEvent& event)
			{
				//функция обработки базового ивента в специальный 
				m_event_dispatcher.dispatch(event);
			}
		);
		//-----------------------------------------------------------------------------

		//пока окно не закрыто цикл рендера работает
		while (!m_bCloseWindow)
		{
			m_pWindow->on_update();
			on_update();
		}
		//автоматически вызываем деструктор окна 
		m_pWindow = nullptr;

		return 0;
	}


}