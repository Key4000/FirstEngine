#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace FirstEngine {

	class Camera
	{
	public:
		//способ проекции 
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};
		//конструктор(позицию камеры, и направление, способ проекции)
		Camera( const glm::vec3& position = { 0, 0, 0 },
				const glm::vec3& rotation = { 0, 0, 0 },
				const ProjectionMode projection_mode = ProjectionMode::Perspective);
		//позиция, поворот 
		void set_position(const glm::vec3& position);
		void set_rotation(const glm::vec3& rotation);
		void set_position_rotation(const glm::vec3& position, const glm::vec3& rotation);
		//установка способа проекции
		void set_projection_mode(const ProjectionMode projection_mode);
		//взять матрицу камеры 
		glm::mat4 get_view_matrix() const { return m_view_matrix; }
		//взять проекцию 
		glm::mat4 get_projection_matrix() const { return m_projection_matrix; }

		//движение вперед по обзору 
		void move_forward(const float delta);
		//движение в бок по обзору 
		void move_right(const float delta);
		//движение вверх по обзору 
		void move_up(const float delta);

		const glm::vec3& get_camera_position() const { return m_position; }
		const glm::vec3& get_camera_rotation() const { return m_rotation; }

		//и движение и перемещение 
		// movement_delta.x - forward , movement_delta.y - right ,movement_delta.z - up ,
		// rotation_delta.x - roll, rotation_delta.y - pitch , rotation_delta.z - yaw
		void add_movement_and_rotation( const glm::vec3& movement_delta, const glm::vec3& rotation_delta);

	private:
		//обновить 
		void update_view_matrix();
		//обновить 
		void update_projection_matrix();
		//позиция, поворот 
		glm::vec3 m_position;
		glm::vec3 m_rotation;        //X - Roll, Y - Pitch, Z - Yaw 

		//векторы для движения по обзору камеры 
		glm::vec3 m_direction;
		glm::vec3 m_right;
		glm::vec3 m_up;


		//неизменяемые координаты мира 
		static constexpr glm::vec3 s_world_up{0.f, 0.f, 1.f};
		static constexpr glm::vec3 s_world_right{0.f, -1.f, 0.f};
		static constexpr glm::vec3 s_world_forward{1.f, 0.f, 0.f};

		//способ проекции 
		ProjectionMode m_projection_mode;
		//матрица перехода к камере 
		glm::mat4 m_view_matrix;
		//матрица добавления проекции 
		glm::mat4 m_projection_matrix;
	};
}
