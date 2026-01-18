#pragma once

struct GLFWwindow;

namespace FirstEngine {
	class VertexArray;

	class Renderer_OpenGL {
	public:
		//инициализация рендера
		static bool init(GLFWwindow* pWindow);

		//отрисовка принимает буфер с которого рисуем
		static void draw(const VertexArray& vertex_array);
		//задать очистку цвета 
		static void set_clear_color(const float r, const float g, const float b, const float a);
		//очистка цвета
		static void clear();
		//задать viewport(место показа)
		static void set_viewport(const unsigned int width, const unsigned int height, const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);

		//
		static const char* get_vendor_str();
		//
		static const char* get_renderer_str();
		//
		static const char* get_version_str();
	};

}