#include <iostream>
#include <memory>
#include <FirstEngineCore/Application.hpp>
#include <imgui/imgui.h>
#include <FirstEngineCore/Input.hpp>

class FirstEngineEditor : public FirstEngine::Application
{
    //каждый кадр в основном цикле!!! 
	virtual void on_update()  override
	{
		//-----------------движение камеры по клавишам--------------//
       if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_W))
        {
            camera_position[2] -= 0.01f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_S))
        {
            camera_position[2] += 0.01f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_A))
        {
            camera_position[0] -= 0.01f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_D))
        {
            camera_position[0] += 0.01f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_E))
        {
            camera_position[1] += 0.01f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_Q))
        {
            camera_position[1] -= 0.01f;
        }

        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_UP))
        {
            camera_rotation[0] += 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_DOWN))
        {
            camera_rotation[0] -= 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_RIGHT))
        {
            camera_rotation[1] -= 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_LEFT))
        {
            camera_rotation[1] += 0.5f;
		}
		//------------------------------------------------------------------//
		
		virtual void on_ui_draw() override
    {
        ImGui::Begin("Editor");
        ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
        ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);
        ImGui::Checkbox("Perspective camera", &perspective_camera);
        ImGui::End();
    }


	}

	int frame = 0;
};

int main()
{
	auto pFirstEngineEditor = std::make_unique<FirstEngineEditor>();

	int returnCode = pFirstEngineEditor->start(1024, 768, "FirstEngine Editor");

	//std::cin.get();

	return returnCode;
}
