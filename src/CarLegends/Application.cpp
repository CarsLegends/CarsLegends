#include <cassert>
#include <chrono>
#include <iostream>

#include "Game.hpp"
#include "Resources/raudio/raudio.h"

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

	int counter = 0;
	float deltaTime = 0.0f;
	auto previousTime = high_resolution_clock::now();
	InitAudioDevice();
	Sound fx = LoadSound("Resources/music/BeatOne.mp3");
	while (game.IsRunning())
	{
		auto startTimeFrame = high_resolution_clock::now();
		const float timeDifference = std::chrono::duration<float>(startTimeFrame - previousTime).count();
		counter++;

		if (timeDifference >= 1.0)
		{
			std::string fps = std::to_string(1.0 / timeDifference * counter);
			std::string ms = std::to_string(timeDifference / counter * 1000);

			std::cout << "FPS: " << fps << ", ms: " << ms << std::endl;

			previousTime = startTimeFrame;
			counter = 0;
		}
		PlaySound(fx);
		game.Update(deltaTime);

		auto stopTimeFrame = high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(stopTimeFrame - startTimeFrame).count();
	}
	UnloadSound(fx); 
	CloseAudioDevice(); 
	return 0;
}
