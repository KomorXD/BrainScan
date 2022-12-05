#pragma once

#include <chrono>

class Timer
{
	private:
		std::chrono::steady_clock::time_point m_T1;
		std::chrono::steady_clock::time_point m_T2;

		std::string m_Name;

	public:
		Timer(const std::string& taskName);
		~Timer();
};

#ifdef BS_DEBUG
	#define SCOPE_PROFILE(name) Timer t##__LINE__(name)
	#define FUNC_PROFILE() Timer t##__func__(__func__)
#else
	#define SCOPE_PROFILE(name)
	#define FUNC_PROFILE()
#endif