#pragma once

namespace FirstEngine{
    class ShaderProgram
    {
    public:
        //конструктор основной
        ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
        //конструктор перемещения 
        ShaderProgram(ShaderProgram&&);
        //оператор присваивания с перемещением 
        ShaderProgram& operator=(ShaderProgram&&);
        ~ShaderProgram();

        //удаляем 
        //конструктор по умолчанию 
        ShaderProgram() = delete;
        //конструктор копирования
        ShaderProgram(const ShaderProgram&) = delete;
        //оператор присваивания с копированием 
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        //делает этот шейдер текущим 
        void bind() const;
        //делает этот шейдер не текущим
        static void unbind();
        //проверка на скомпилированность
        bool isCompiled() const { return m_is_compiled; }
    private:
        bool m_is_compiled = false;
        unsigned int m_id = 0;
    };

}