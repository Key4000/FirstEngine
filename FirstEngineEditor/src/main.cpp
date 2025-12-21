#include <iostream>

#include <FirstEngineCore/Utils/test.hpp>

int main()
{
	std::cout << "Hello from Simple Engine Editor " << std::endl;

	FirstEngine::checkGLFW();

	std::cin.get();
}