#include "FirstEngineCore/Camera.hpp"

#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

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
	
	//отдать позицию камеры
	const glm::mat4& Camera::get_view_matrix()
	{
		if (m_update_view_matrix) 
		{
			update_view_matrix();
		}
		return m_view_matrix;
	}

	//обновить позицию камеры 
	void Camera::update_view_matrix()
	{
		//значения поворота камеры по обзору
		const float roll_in_radians = glm::radians(m_rotation.x);
		const float pitch_in_radians = glm::radians(m_rotation.y);
		const float yaw_in_radians = glm::radians(m_rotation.z);

		//матрица поворота по x 
		const glm::mat3 rotate_matrix_x(1, 0, 0,
			0, cos(roll_in_radians), sin(roll_in_radians),
			0, -sin(roll_in_radians), cos(roll_in_radians)
		);

		//матрица поворота по y
		const glm::mat3 rotate_matrix_y(
			cos(pitch_in_radians), 0, -sin(pitch_in_radians),
			0, 1, 0,
			sin(pitch_in_radians), 0, cos(pitch_in_radians)
		);

		//матрица поворота по z 
		const glm::mat3 rotate_matrix_z(
			cos(yaw_in_radians), sin(yaw_in_radians), 0,
			-sin(yaw_in_radians), cos(yaw_in_radians), 0,
			0, 0, 1
		);

		//матрица Эйлера (соединение всех поворотов в один)(порядок важен, справа налево!)
		const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;

		//вектор вперед
		m_direction = glm::normalize(euler_rotate_matrix * s_world_forward);
		//вектор вправо
		m_right = glm::normalize(euler_rotate_matrix * s_world_right);
		//вверх
		m_up = glm::cross(m_right, m_direction);

		//строим матрицу перехода к камере
		m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_up);
	}

	//обновить проекцию камеры
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

	//установить позицю
	void Camera::set_position(const glm::vec3& position)
	{
		m_position = position;
		m_update_view_matrix = true;
	}

	//поворот
	void Camera::set_rotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
		m_update_view_matrix = true;
	}

	//позиция + поворот 
	void Camera::set_position_rotation(const glm::vec3& position, const glm::vec3& rotation)
	{
		m_position = position;
		m_rotation = rotation;
		//вызываем один раз, поэтому вынесли эту функцию отдельно 
		m_update_view_matrix = true;
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
		m_update_view_matrix = true;
	}

	//движение в бок по обзору 
	void Camera::move_right(const float delta)
	{
		m_position += m_right * delta;
		m_update_view_matrix = true;
	}
	//движение вверх по обзору 
	void Camera::move_up(const float delta)
	{
		m_position += m_up * delta;
		m_update_view_matrix = true;
	}
	void Camera::add_movement_and_rotation(const glm::vec3& movement_delta, const glm::vec3& rotation_delta)
	{
		//текущая координата + смещение * вектор
		m_position += movement_delta.x * m_direction;
		m_position += movement_delta.y * m_right;
		m_position += movement_delta.z * m_up;
		//поворот
		m_rotation += rotation_delta;
		m_update_view_matrix = true;
	}
}
