#ifndef EVENTS_EVENT_TYPES_HPP
#define EVENTS_EVENT_TYPES_HPP

#include "../DataStructures/Types.hpp"
#include "../Macros/Hash.hpp"

namespace Events
{
	using namespace DataStructures;
	using namespace Macros;

	// The string as long as possible to reduce collisions
	constexpr EventId WINDOW_QUIT = "Events::Window::QUIT"_hash;
	constexpr EventId WINDOW_RESIZED = "Events::Window::RESIZED"_hash;
	constexpr EventId WINDOW_INPUT = "Events::Window::INPUT"_hash;
}

#endif
