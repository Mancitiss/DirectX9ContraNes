#pragma once
#include <Windows.h>

class GameTime
{
public:
	float elapsedGameTime;
	float totalGameTime;

	bool Initialize();
	void Update();

private:
	LONGLONG start;
	float frequencyPerSecond;
};