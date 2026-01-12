#pragma once
#include "VertexBuffer.hpp"

namespace SimpleEngine {

 class IndexBuffer {
 public:
    //
    IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage = VertexBuffer::EUsage::Static);
    //
    ~IndexBuffer();
   
    //удаляем конструктор с копипованием 
    IndexBuffer(const IndexBuffer&) = delete;
    //удаляем оператор присваивание с копированием 
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    
    //оператор присваиваивания с перемещением не выбрасывает исключения
    IndexBuffer& operator=(IndexBuffer&& index_buffer) noexcept;
    //конструктор с перемещением не выбрасывает исключения 
    IndexBuffer(IndexBuffer&& index_buffer) noexcept;

    //сделать текущим 
    void bind() const;
    //убрать из текущего 
    static void unbind();
    //получить кол-во эл-тов в этом буфере(вертексов)
    size_t get_count() const { return m_count; }

private:
    //id буфера 
    unsigned int m_id = 0;
    //кол-во эл-тов в буфере (сколько вертексов)
    size_t m_count;
 };

}
