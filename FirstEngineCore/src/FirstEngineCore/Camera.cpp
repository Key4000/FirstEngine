#include "FirstEngineCore/Camera.hpp"

#include <glm/trigonometric.hpp>

namespace FirstEngine {

	//конструктор
	Camera::Camera(const glm::vec3& position,
		const glm::vec3& rotation,
		const ProjectionMode projection_mode)
		: m_position(position)
		, m_rotation(rotation)
		, m_projection_mode(projection_mode)
	{
		//перейти к камере
		update_view_matrix();
		//добавить проекцию 
		update_projection_matrix();
	}

	//обновить матрицу камеры 
	void Camera::update_view_matrix()
	{
		//значения поворота камеры по обзору
		const float roll_in_radians = glm::radians(m_rotations.x);
		const float pitch_in_radians = glm::radians(m_rotations.y);
        const float yaw_in_radians = glm::radians(m_rotations.z);		

		//матрица поворота по x 
		const glm::mat4 rotate_matrix_x(
			1, 0, 0, 0,
			0, cos(roll_in_radians), sin(roll_in_radians), 0,
			0, -sin(roll_in_radians), cos(roll_in_radians), 0,
			0, 0, 0, 1);

		//матрица поворота по y
		const glm::mat4 rotate_matrix_y(
			cos(pitch_in_radians), 0, -sin(pitch_in_radians), 0,
			0, 1, 0, 0,
			sin(pitch_in_radians), 0, cos(pitch_in_radians), 0,
			0, 0, 0, 1);

		//матрица поворота по z 
		const glm::mat4 rotate_matrix_z(
			cos(yaw_in_radians), sin(yaw_in_radians), 0, 0,
			-sin(yaw_in_radians), cos(yaw_in_radians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		
        //матрица Эйлера (соединение всех поворотов в один)(порядок важен, справа налево!)
		const glm::mat4 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;

		m_view_matrix = rotate_matrix_y * rotate_matrix_x * translate_matrix;
	}

	//обновить матрицу проекции 
	void Camera::update_projection_matrix()
	{
		if (m_projection_mode == ProjectionMode::Perspective)
		{
			//viewing frustum для переспективы  
			float r = 0.1f;
			float t = 0.1f;
			float f = 10;
			float n = 0.1f;
			m_projection_matrix = glm::mat4(n / r, 0, 0, 0,
				0, n / t, 0, 0,
				0, 0, (-f - n) / (f - n), -1,
				0, 0, -2 * f * n / (f - n), 0);
		}
		else
		{
			//viewing frustum для ортографической 
			float r = 2;
			float t = 2;
			float f = 100;
			float n = 0.1f;
			m_projection_matrix = glm::mat4(1 / r, 0, 0, 0,
				0, 1 / t, 0, 0,
				0, 0, -2 / (f - n), 0,
				0, 0, (-f - n) / (f - n), 1);
		}
	}

	//позиция 
	void Camera::set_position(const glm::vec3& position)
	{
		m_position = position;
		update_view_matrix();
	}

	//поворот
	void Camera::set_rotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
		update_view_matrix();
	}

	//позиция + поворот 
	void Camera::set_position_rotation(const glm::vec3& position, const glm::vec3& rotation)
	{
		m_position = position;
		m_rotation = rotation;
		//вызываем один раз, поэтому вынесли эту функцию отдельно 
		update_view_matrix();
	}

	//проекция    
	void Camera::set_projection_mode(const ProjectionMode projection_mode)
	{
		m_projection_mode = projection_mode;
		update_projection_matrix();
	}

    //движение вперед по обзору 
    void Camera::move_forward(const float delta) 
    {
         m_position += m_direction * delta;
	}

    //движение в бок по обзору 
    void Camera::move_right(const float delta) 
    {
         m_position += m_right * delta;
    }  
    //движение вверх по обзору 
    void Camera::move_up(const float delta) 
    {
         m_position += m_up * delta;
    } 
}
