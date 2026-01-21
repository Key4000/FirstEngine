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
		Camera(const glm::vec3& position = { 0, 0, 0 },
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

		//способ проекции 
		ProjectionMode m_projection_mode;
		//матрица перехода к камере 
		glm::mat4 m_view_matrix;
		//матрица добавления проекции 
		glm::mat4 m_projection_matrix;
	};
}
