#ifndef CONFIGURATION_WINDOW_CONFIGURATION_HPP
#define CONFIGURATION_WINDOW_CONFIGURATION_HPP
#include <cstdint>

namespace Configuration
{
	enum class Buttons
	{
		W,
		A,
		S,
		D,
		Space,
		LeftShift
	};

	constexpr uint32_t WINDOW_HEIGHT = 800;
	constexpr uint32_t WINDOW_WIDTH = 800;
	constexpr uint32_t WINDOW_BUTTONS_COUNT = 6;
	const std::string WINDOW_TITLE = "CarLegends";
}

#endif