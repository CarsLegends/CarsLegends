#ifndef CONFIGURATION_MANAGERS_CONFIGURATION_HPP
#define CONFIGURATION_MANAGERS_CONFIGURATION_HPP
#include <cstdint>

namespace Configuration
{
	constexpr uint32_t MAX_ENTITIES = 1000;
	constexpr uint8_t MAX_COMPONENTS = 32;

	// SHOULD BE IN ITS OWN CONFIGURATION
	constexpr uint8_t MAX_LIGHT_SOURCE = 5;
}

#endif