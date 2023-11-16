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
	constexpr uint32_t WINDOW_BUTTONS_COUNT = 6;
	const std::string WINDOW_TITLE = "CarLegends";
}

#endif