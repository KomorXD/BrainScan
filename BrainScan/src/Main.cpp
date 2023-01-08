#include "App.hpp"
#include "Core.hpp"

int main(int argc, char** argv)
{
	Logger::Init();
	
	App::GetInstance().Run();	

	return 0;
}