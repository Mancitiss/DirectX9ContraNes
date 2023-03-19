#include "GameTime.h"

bool GameTime::Initialize()
{
	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
		return false;

	// number of ticks per second
	frequencyPerSecond = (float)frequency.QuadPart;

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	start = time.QuadPart;

	elapsedGameTime = 0.0f;
	totalGameTime = 0.0f;

	return true;
}

void GameTime::Update()
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	LONGLONG current = time.QuadPart;

	elapsedGameTime = (float)(current - start) / frequencyPerSecond;

	start = time.QuadPart;
	totalGameTime += elapsedGameTime;
}