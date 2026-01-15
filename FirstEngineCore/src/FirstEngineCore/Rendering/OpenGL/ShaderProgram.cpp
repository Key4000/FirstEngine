#include "ShaderProgram.hpp";

#include "FirstEngineCore/Log.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace FirstEngine {
	//вспомогательная функция для создания шейдера 
	bool create_shader(const char* source, const GLenum shader_type, GLuint& shader_id) {

		shader_id = glCreateShader(shader_type);   //создаем шейдер по его типу 
		glShaderSource(shader_id, 1, &source, nullptr);   //передача кода шейдеру по его id 
		glCompileShader(shader_id);    //компиляция шейдера по его id


		GLint success;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success); //берем параметр любой параметр (сейчас GL_COMPILE_STATUS) из шейдера и пишем её в переменную success
		//если компиляция прошла неудачно 
		if (success == GL_FALSE) {
			char info_log[1024];
			glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);

			LOG_CRITICAL("Shader compilation error: \n{0}", info_log);
			return false;
		}
		return true;
	}


	ShaderProgram::ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src)
	{
		//создаем вертексный шейдер
		GLuint vertex_shader_id = 0;
		if (!(create_shader(vertex_shader_src, GL_VERTEX_SHADER, vertex_shader_id)))
		{
			LOG_CRITICAL("VERTEX SHADER: Compile-time error!");
			glDeleteShader(vertex_shader_id);
			return;
		}

		//создаем фрагментный шейдер
		GLuint fragment_shader_id = 0;
		if (!create_shader(fragment_shader_src, GL_FRAGMENT_SHADER, fragment_shader_id))
		{
			LOG_CRITICAL("FRAGMENT SHADER: Compile-time error!");
			//удаляем так же и вертексный, так как он теперь не нужен
			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);
			return;
		}

		//шейдерная программа 
		m_id = glCreateProgram();
		glAttachShader(m_id, vertex_shader_id);
		glAttachShader(m_id, fragment_shader_id);
		glLinkProgram(m_id);


		GLint success;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLchar info_log[1024];
			glGetProgramInfoLog(m_id, 1024, nullptr, info_log);
			LOG_CRITICAL("SHADER PROGRAM: Link-time error:\n{0} ")
				glDeleteProgram(m_id);
			m_id = 0;
			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);
			return;
		}
		else
		{
			m_is_compiled = true;
		}

		//открепляем шейдер от программы
		glDetachShader(m_id, vertex_shader_id);
		glDetachShader(m_id, fragment_shader_id);
		//теперь можно удалять 
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
	}

	//деструктор  
	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_id);
	}

	//сделать текущей 
	void ShaderProgram::bind() const {
		glUseProgram(m_id);
	}

	//убираем текущую 
	void ShaderProgram::unbind() {
		glUseProgram(0);
	}

	//перемещение 
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shader_program) {
		glDeleteProgram(m_id);
		m_id = shader_program.m_id;
		m_is_compiled = shader_program.m_is_compiled;

		//затираем аргумент, чтобы два раза не вызвался деструктор
		shader_program.m_id = 0;
		shader_program.m_is_compiled = false;
		return *this;
	}

	//конструктор перемещения 
	ShaderProgram::ShaderProgram(ShaderProgram&& shader_program)
	{
		m_id = shader_program.m_id;
		m_is_compiled = shader_program.m_is_compiled;
		//затираем аргумент, чтобы два раза не вызвался деструктор
		shader_program.m_id = 0;
		shader_program.m_is_compiled = false;
	}
//устанавливаем матрицу в шейдерную программу
 void ShaderProgram::setMatrix4(const char* name, const glm::mat4& matrix) const
 {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
  }

}