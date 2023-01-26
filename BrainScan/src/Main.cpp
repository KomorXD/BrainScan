#include "App.hpp"
#include "Core.hpp"
#include <Windows.h>

int main(int argc, char** argv)
{
#ifdef BS_PROD
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#else
	ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif

	Logger::Init();
	
	App::GetInstance().Run();	

	return 0;
}