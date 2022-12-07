#include "App.hpp"
#include "Core.hpp"
#include "scan/Scan.hpp"

int main(int argc, char** argv)
{
	Logger::Init();
	//App::GetInstance().Run();	

	Scan s;
	s.LoadFromFile("D:\\Projekty\\BrainScan\\NiftiFiles\\avg152T1_LR_nifti.nii");

	return 0;
}