/*#include "DebugFPS.h"
#include <ctime>
#include <chrono>
#include "Logger.h"

double clockToMilliseconds(clock_t ticks)
{
	return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}

DebugFPS::DebugFPS(unsigned int* ptrToFrameCount, clock_t* ptrToDeltaTime)
{
	_frames = ptrToFrameCount;
	_deltaTime = ptrToDeltaTime;
}

DebugFPS::~DebugFPS()
{
	_frames = nullptr;
	_deltaTime = nullptr;
}

void DebugFPS::update()
{
	//Debug_Logger->TypeDebugPrintF(Logger_Info, "%4.6f", clockToMilliseconds(*_deltaTime));

	if (clockToMilliseconds(*_deltaTime) > 1000.0)	//should happen every second
	{
		_frameRate = (double)*_frames * 0.5 + _frameRate * 0.5;
		*_frames = 0;
		_deltaTime -= CLOCKS_PER_SEC;
		_averageFramesTime = 1000.0 / (_frameRate == 0 ? 0.001 : _frameRate);

		Debug_Logger->TypeDebugPrintF(Logger_Info, "CPU time was: %0.7f\n", _averageFramesTime);
	}
}*/
