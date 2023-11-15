#include <cassert>
#include <chrono>

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

	float deltaTime = 0.0f;
	time_point<steady_clock> lastFrameTime;
	while (game.IsRunning())
	{
		auto currentFrameTime = high_resolution_clock::now();
		deltaTime = duration<float>(lastFrameTime - currentFrameTime).count();
		lastFrameTime = currentFrameTime;

		game.Update(deltaTime);
	}

	return 0;
}
