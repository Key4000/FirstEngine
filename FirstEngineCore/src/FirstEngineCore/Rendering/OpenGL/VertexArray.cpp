#include "VertexArray.hpp";
#include "FirstEngineCore/Log.hpp";
#include <glad/glad.h>

namespace FirstEngine{

    //конструктор 
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_id);
    };

    //деструктор
    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_id);
    };

    //присваивание с перемещением не выбрасываются исключения
    VertexArray& VertexArray::operator=(VertexArray&& vertex_array) noexcept {
        m_id = vertex_array.m_id;
        m_elements_count = vertex_array.m_elements_count;

        vertex_array.m_id = 0;
        vertex_array.m_elements_count = 0;

        return *this;
    };
    //конструктор с перемещением не выбрасываются исключения
    VertexArray::VertexArray(VertexArray&& vertex_array) noexcept:
        m_id(vertex_array.m_id)
        , m_elements_count(vertex_array.m_elements_count)
    {
        vertex_array.m_id = 0;
        vertex_array.m_elements_count = 0;
    };


    void VertexArray::add_vertex_buffer(const VertexBuffer& vertex_buffer) {
        bind(); //делаем наш vertex array текущим
        vertex_buffer.bind();   //делаем текущим vertex buffer

        glEnableVertexAttribArray(m_elements_count); //включаем m_elements_count позицию
        glVertexAttribPointer(m_elements_count, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    //

        //проходим по всем эл-там окружения
        for (const BufferElement& current_element : vertex_buffer.get_layout().get_elements())
        {          
            glEnableVertexAttribArray(m_elements_count);        //включаем m_elements_count позицию
            glVertexAttribPointer(
                m_elements_count,
                static_cast<GLint>(current_element.components_count),
                current_element.component_type,
                GL_FALSE,
                static_cast<GLsizei>(vertex_buffer.get_layout().get_stride()),
                reinterpret_cast<const void*>(current_element.offset)
            );
            ++m_elements_count;
        };

        void  VertexArray::set_index_buffer(const IndexBuffer& index_buffer)
       {
           bind();
           index_buffer.bind();
           m_indices_count = index_buffer.get_count();
       } 

    };
    void VertexArray::bind() const
    {
        glBindVertexArray(m_id);   //сделать текущим 
    };
    void VertexArray::unbind()
    {
        glBindVertexArray(0);   //сделать текущим
    };
}