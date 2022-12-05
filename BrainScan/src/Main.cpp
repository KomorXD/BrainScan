#include "App.hpp"
#include "Core.hpp"
#include "SimpleITK-2.3/sitkImage.h"

int main(int argc, char** argv)
{
	Logger::Init();

	{
		FUNC_PROFILE();

		{
			SCOPE_PROFILE("Logging");

			LOG_TRACE("Trace log: {0} = {1}", 'a', 1);
			LOG_INFO("Info log: {} = {}", 'b', 2);
			LOG_WARN("Warn log: {0} = {1}", 'c', 3);
			LOG_ERROR("Error log: {0} = {1}", 'x', 4);
			LOG_CRITICAL("Critical log: {0} = {1}", 'y', 5);
		}
	}

	itk::simple::Image a;
	
	LOG_INFO("{}", a.GetWidth());

	App::GetInstance().Run();
	
	return 0;
}