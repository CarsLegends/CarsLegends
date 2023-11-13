#include <cassert>

#include "Game.hpp"

Coordinators::Coordinator gCoordinator();

int main() {
	assert(__cplusplus != 201703L && "The application requires of ISO C++17 Standard (/std:c++17)");

	Game::Game game{};

	game.RegisterComponents();
	game.RegisterSystems();
	game.RegisterEntities();

	while(game.IsRunning()) {
		game.Update();
	}

	return 0;
}
