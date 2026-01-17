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
        update_view_matrix()
        //добавить проекцию 
        update_projection_matrix();
    }
    
    //обновить матрицу камеры 
    void Camera::update_view_matrix()
    {
        //угол в радианах по x
        float rotate_in_radians_x = glm::radians(-m_rotation.x);
        //матрица поворота по x 
        glm::mat4 rotate_matrix_x(1, 0, 0, 0,
            0, cos(rotate_in_radians_x), sin(rotate_in_radians_x), 0,
            0, -sin(rotate_in_radians_x), cos(rotate_in_radians_x), 0,
            0, 0, 0, 1);
        //угол в радианах по y 
        float rotate_in_radians_y = glm::radians(-m_rotation.y);
        //матрица поворота по y
        glm::mat4 rotate_matrix_y(cos(rotate_in_radians_y), 0, -sin(rotate_in_radians_y), 0,
            0, 1, 0, 0,
            sin(rotate_in_radians_y), 0, cos(rotate_in_radians_y), 0,
            0, 0, 0, 1);
        //угол поворота в радианах по z
        float rotate_in_radians_z = glm::radians(-m_rotation.z);
        //матрица поворота по z 
        glm::mat4 rotate_matrix(cos(rotate_in_radians_z), sin(rotate_in_radians_z), 0, 0,
            -sin(rotate_in_radians_z), cos(rotate_in_radians_z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
        //матрица перемещения 
        glm::mat4 translate_matrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -m_position[0], -m_position[1], -m_position[2], 1);

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
}
