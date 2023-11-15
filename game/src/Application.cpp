#include <cassert>
#include <iostream>
#include <string>

#include "Game.hpp"



int main()
{
    assert(__cplusplus != 201703L && __cplusplus != 201703 && "The application requires of ISO C++17 Standard (/std:c++17)");
    
	Game::Game game{};

	game.RegisterComponents();
	game.RegisterSystems();
	game.RegisterEntities();

	while (game.IsRunning())
	{
		game.Update();
	}

	return 0;
}
