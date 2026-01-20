#include "FirstEngineCore/Application.hpp"
#include "FirstEngineCore/Log.hpp"
#include "FirstEngineCore/Window.hpp"
#include "FirstEngineCore/Event.hpp"
#include "FirstEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "FirstEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "FirstEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "FirstEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "FirstEngineCore/Camera.hpp"
#include "FirstEngineCore/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "FirstEngineCore/Modules/UIModule.hpp"
#include "FirstEngineCore/Input.hpp"

#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>


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
    float m_background_color[4] = { 0.f, 0.f, 0.f, 0.f }; //для хранения цвета фона
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

		
             //слушатель нажатия клавиши
		        m_event_dispatcher.add_event_listener<EventKeyPressed>(
            [&](EventKeyPressed& event)
            {
                if (event.key_code <= KeyCode::KEY_Z)
                {
                    if (event.repeated)
                    {
                        LOG_INFO("[Key pressed: {0}, repeated", static_cast<char>(event.key_code));
                    }
                    else
                    {
                        LOG_INFO("[Key pressed: {0}", static_cast<char>(event.key_code));
                    }
                }
                Input::PressKey(event.key_code);
            });
        //слушатель отжима кнопки
        m_event_dispatcher.add_event_listener<EventKeyReleased>(
            [&](EventKeyReleased& event)
            {
                if (event.key_code <= KeyCode::KEY_Z)
                {
                    LOG_INFO("[Key released: {0}", static_cast<char>(event.key_code));
                }
                Input::ReleaseKey(event.key_code);
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

        //-----------------------------------------------------------------------------
		//РАБОТА С ШЕЙДЕРАМИ
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
		   
		//пока окно не закрыто цикл рендера работает
		while (!m_bCloseWindow)
		{
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
		ImGui::Checkbox("Perspective camera", &perspective_camera);					//перспектива                	
		ImGui::End();                                               //закрытие окна
		//кастомный ui из Editor
		on_ui_draw();
			
        UIModule::on_ui_draw_end()
        m_pWindow->on_update();
	    on_update();
		}
		//автоматически вызываем деструктор окна 
		m_pWindow = nullptr;

		return 0;
	}


}
