#pragma once 

#include <cinttypes>
#include <initializer_list>
#include <vector>

namespace FirstEngine{

    enum class ShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
    };

    //класс окружения буффера
    struct BufferElement
    {
        
        uint32_t component_type;
        size_t components_count;
        size_t size;
        size_t offset;
        ShaderDataType type;

        BufferElement(const ShaderDataType type);
    };

    class BufferLayout
    {
    public:
        //передаем эл-ты которые находятся в окружении
        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_elements(std::move(elements))
        {
            //считаем сдвиг и шаг
            size_t offset = 0;
            m_stride = 0;
            for (auto& element : m_elements)
            {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }

        //получить эл-ты
        const std::vector<BufferElement>&get_elements() const { return m_elements; };
        //получить страйд
        size_t get_stride() const { return m_stride; };

    private:
        //вектор элементов 
        std::vector<BufferElement> m_elements;
        //через какое кол-во байт идет след эл-т(шаг)
        size_t m_stride = 0;
    };

    class VertexBuffer {
    public:
        //перечисление для абстрагирования от конкретного API
        enum class EUsage {
            Static,
            Dynamic,
            Stream
        };

        //конструктор(ссылка на vertex buffer objlect, размер буффера, инфа об упаковке данных(окружение), usage) 
        VertexBuffer(const void* data, const size_t size, BufferLayout m_buffer_layout, const EUsage usage = VertexBuffer::EUsage::Static);
        //деструктор 
        ~VertexBuffer();

        //удаляем конструктор копирования
        VertexBuffer(const VertexBuffer&) = delete;
        //удаляем оператор присваивания с копированием 
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        //оператор присваивания с перемещением не выбрасывает исключений
        VertexBuffer& operator=(VertexBuffer&& vertex_buffer) noexcept;
        //конструктор с перемещением не выбрасывает исключений
        VertexBuffer(VertexBuffer&& vertex_buffer) noexcept;

        //делает этот шейдер текущим 
        void bind() const;
        //делает этот шейдер не текущим
        static void unbind();

        //получить информацию об упаковке данных в буфере(окружение)
        const BufferLayout& get_layout() const { return m_buffer_layout; }

    private:
        //идентификатор буффера
        unsigned int m_id = 0;
        //как данные упакованы в этом буфере(окружение)
        BufferLayout m_buffer_layout;
    };
}