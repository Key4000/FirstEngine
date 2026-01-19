#include <iostream>
#include <memory>
#include <FirstEngineCore/Application.hpp>

class FirstEngineEditor : public FirstEngine::Application
{
	virtual void on_update()  override
	{
		//std::cout << "Update frame: " << frame++ << std::endl;
	}

	int frame = 0;
};

int main()
{
	auto pFirstEngineEditor = std::make_unique<FirstEngineEditor>();

	int returnCode = pFirstEngineEditor->start(1024, 768, "FirstEngine Editor");

	std::cin.get();

	return returnCode;
}