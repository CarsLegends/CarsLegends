#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>

#include "Game.hpp"

using namespace std::chrono;

int main()
{
	assert(__cplusplus != 201703L && "The application requires of ISO C++17 Standard (/std:c++17)");

	Game::Game game{};
	{
		game.RegisterListeners();
		game.RegisterComponents();
		game.RegisterSystems();
		game.RegisterEntities();
	}

	float counter = 0;
	float deltaTime = 0.0f;
	auto previousTime = high_resolution_clock::now();
	while (game.IsRunning())
	{
		auto startTimeFrame = high_resolution_clock::now();
		const float timeDifference = std::chrono::duration<float>(startTimeFrame - previousTime).count();
		counter++;

		if (timeDifference >= 1.0)
		{
			std::string fps = std::to_string(static_cast<int>(1.0f / timeDifference * counter));
			std::string ms = std::to_string(timeDifference / counter * 1000.0f).substr(0,5);

			game.UpdateFPS("FPS: " + fps + ", ms: " + ms);

			previousTime = startTimeFrame;
			counter = 0;
		}

		game.Update(deltaTime);

		auto stopTimeFrame = high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(stopTimeFrame - startTimeFrame).count();
	}

	return 0;
}
