#include <iostream>
#include <memory>
#include <FirstEngineCore/Application.hpp>
#include <imgui/imgui.h>

class FirstEngineEditor : public FirstEngine::Application
{
	virtual void on_update()  override
	{
		//std::cout << "Update frame: " << frame++ << std::endl;
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
