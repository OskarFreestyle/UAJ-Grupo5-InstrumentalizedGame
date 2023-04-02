#include "Timer.h"

Timer* Timer::instance = nullptr;

Timer* TimeManager::Instance()
{
	if (instance == nullptr) instance = new Timer();
	return instance;
}

void Timer::Release()
{
	if (instance != nullptr) 
	{
		delete instance; 
		instance = nullptr;
	}
}

Time::Time()
{
	startTime = time(NULL);
}

TimeManager::~TimeManager()
{
}

double TimeManager::getTimeSinceBeginning()
{
	return difftime(time(NULL), startTime);
}