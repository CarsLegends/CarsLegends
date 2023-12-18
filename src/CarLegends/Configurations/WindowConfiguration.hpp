#ifndef CONFIGURATION_WINDOW_CONFIGURATION_HPP
#define CONFIGURATION_WINDOW_CONFIGURATION_HPP
#include <cstdint>

namespace Configuration
{

	constexpr uint32_t WINDOW_HEIGHT = 720;
	constexpr uint32_t WINDOW_WIDTH = 1280;
	
	const std::string WINDOW_TITLE = "CarLegends";

	constexpr uint32_t WINDOW_BUTTONS_COUNT = 6;
	constexpr uint32_t WINDOW_CONTROLLER_BUTTONS_COUNT = 14;
	constexpr uint32_t WINDOW_PLAYER_COUNT = 2;
	constexpr uint32_t WINDOW_SAMPLES_COUNT = 8;

	constexpr int WINDOW_FONT_WIDTH = 0;
}

#endif