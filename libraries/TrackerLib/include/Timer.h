#pragma once
#include <list>
#include <time.h>

class Timer
{
private:
	static TimeManager* instance;
	time_t startTime;

	Timer();
	~Timer();
public:
	static Timer* Instance();
	static void Release();

	float getTimeSinceStart();
};

