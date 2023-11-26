#ifndef COMPONENTS_PLAYABLE_HPP
#define COMPONENTS_PLAYABLE_HPP
#include <cassert>

#include "../Configurations/WindowConfiguration.hpp"

namespace Components
{
	using namespace Configuration;

	struct Playable
	{
		unsigned int mPlayerNumber;

		Playable();
		Playable(unsigned int playerNumber);
	};

	inline Playable::Playable()
	{
		this->mPlayerNumber = 0;
	}

	inline Playable::Playable(unsigned int playerNumber)
	{
		assert(playerNumber < WINDOW_PLAYER_COUNT && "Unsupported player number");

		this->mPlayerNumber = playerNumber;
	}
}

#endif