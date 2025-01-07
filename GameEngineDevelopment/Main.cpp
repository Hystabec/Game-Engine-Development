#include "Game.h"

#include <iostream>
#include <chrono>

#include "MemoryTracker.h"

int main()
{
	Game* game = new Game();

	if (game)
	{	
		PrintMemoryUsage();

		while (game->GetGameRunning())
		{
			s_AllocationMetrics.Reset();
			game->Update();

			PrintMemoryUsage();
			s_AllocationMetrics.Reset();
		}
		delete game;
		game = nullptr;
	}
	PrintMemoryUsage();

	return 0;
}