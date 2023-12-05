#ifndef CONFIGURATION_WINDOW_CONFIGURATION_HPP
#define CONFIGURATION_WINDOW_CONFIGURATION_HPP
#include <cstdint>

namespace Configuration
{
	const enum class Buttons
	{
		W,
		A,
		S,
		D,
		Space,
		LeftShift
	};

	constexpr uint32_t WINDOW_HEIGHT = 1080;
	constexpr uint32_t WINDOW_WIDTH = 1920;
	
	const std::string WINDOW_TITLE = "CarLegends";

	// Temporal, this should be moved to its own config
	enum class ControllerButtons : int
	{
		A,
		B,
		X,
		Y,
		LB,
		RB,
		Back,
		Start,
		L3,
		R3,
		Up,
		Right,
		Down,
		Left
	};

	constexpr uint32_t WINDOW_BUTTONS_COUNT = 6;
	constexpr uint32_t WINDOW_CONTROLLER_BUTTONS_COUNT = 14;
	constexpr uint32_t WINDOW_PLAYER_COUNT = 2;
	constexpr uint32_t WINDOW_SAMPLES_COUNT = 8;
}

#endif