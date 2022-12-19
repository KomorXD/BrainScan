#include "App.hpp"
#include "Core.hpp"
#include "scan/Scan.hpp"

int main(int argc, char** argv)
{
	Logger::Init();
	//App::GetInstance().Run();	

	Scan s;
	s.LoadFromFile("D:\\Projekty\\BrainScan\\NiftiFiles\\ircad_e01_orig.nii");

	return 0;
}