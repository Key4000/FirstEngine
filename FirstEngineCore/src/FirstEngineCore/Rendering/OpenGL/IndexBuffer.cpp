#include "IndexBuffer.hpp"

#include "FirstEngineCore/Log.hpp"

#include <glad/glad.h>

namespace FirstEngine {

	//выбор usage абстракция, для возможной замены API 
	constexpr GLenum usage_to_GLenum(const VertexBuffer::EUsage usage)
	{
		switch (usage)
		{
		case VertexBuffer::EUsage::Static:  return GL_STATIC_DRAW;
		case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
		case VertexBuffer::EUsage::Stream:  return GL_STREAM_DRAW;
		}

		LOG_ERROR("Unknown VertexBuffer usage");
		return GL_STREAM_DRAW;
	};

	//конструктор 
	IndexBuffer::IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage)
		: m_count(count)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, usage_to_GLenum(usage));
	};

	//деструктор
	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	};

	//оператор присваивания с перемещением не выбрасывает исключения     
	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& index_buffer) noexcept
	{
		m_id = index_buffer.m_id;
		m_count = index_buffer.m_count;
		index_buffer.m_id = 0;
		index_buffer.m_count = 0;
		return *this;
	};

	//конструктор с присваиванием
	IndexBuffer::IndexBuffer(IndexBuffer&& index_buffer) noexcept
		: m_id(index_buffer.m_id)
		, m_count(index_buffer.m_count)
	{
		index_buffer.m_id = 0;
		index_buffer.m_count = 0;
	};

	//сделать текущим 
	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	};

	//убрать из текущего 
	void IndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	};

}
