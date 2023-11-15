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
	constexpr EventId WINDOW_INPUT_BUTTON = "Events::Window::Input::BUTTON"_hash;
	constexpr EventId WINDOW_INPUT_CURSOR = "Events::Window::Input::CURSOR"_hash;
	constexpr EventId WINDOW_HIDE_CURSOR = "Events::Window::Hide::CURSOR"_hash;
	constexpr EventId WINDOW_SHOW_CURSOR = "Events::Window::Show::CURSOR"_hash;
	constexpr EventId WINDOW_SET_CURSOR_POSITION = "Events::Window::Set::Cursor::Position"_hash;
}

#endif
