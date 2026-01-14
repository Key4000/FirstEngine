#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace FirstEngine {
	class VertexArray {
	public:
		//конструктор 
		VertexArray();
		//деструктор
		~VertexArray();

		//удаляем конструктор с копированием
		VertexArray(const VertexArray&) = delete;
		//удаляем оператор присваивания с копированием
		VertexArray& operator=(const VertexArray&) = delete;


		//оператор присваивания с перемещением не выбрасывает исключения 
		VertexArray& operator=(VertexArray&& vertex_array) noexcept;
		//конструктор с перемещением не выбрасывает исключений 
		VertexArray(VertexArray&& vertex_array) noexcept;


		//привязываем буффер(vertex buffer object) к этому Vertex Array Object
		void add_vertex_buffer(const VertexBuffer& vertex_buffer);

		//устанавливаем один index buffer
		void set_index_buffer(const IndexBuffer& index_buffer);

		//получить кол-во буферов в index buffer
		size_t get_indices_count() const { return m_indices_count; }

		//делает текущим 
		void bind() const;

		//убирает с текущего 
		static void unbind();

	private:

		//id для vertex array object 
		unsigned int m_id = 0;

		//кол-во vbo
		unsigned int m_elements_count = 0;

		//кол-во буферов index buffer 
		size_t m_indices_count = 0;
	};
}