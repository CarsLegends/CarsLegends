#ifndef EVENTS_EVENT_PARAMETERS_HPP
#define EVENTS_EVENT_PARAMETERS_HPP

#include "../DataStructures/Types.hpp"
#include "../Macros/Hash.hpp"

namespace Events
{
	using namespace DataStructures;
	using namespace Macros;

	// The string as long as possible to reduce collisions
	constexpr ParameterId WINDOW_RESIZED_WIDTH = "Events::Window::Resized::WIDTH"_hash;
	constexpr ParameterId WINDOW_RESIZED_HEIGHT = "Events::Window::Resized::HEIGHT"_hash;
	constexpr ParameterId WINDOW_KEYBOARD_INPUT_BUTTON_PARAMETER = "Events::Window::Keyboard::Input::Button::PARAMETER"_hash;
	constexpr ParameterId WINDOW_CONTROLLER_INPUT_PARAMETER = "Events::Window::Controller::Input::PARAMETER"_hash;
	constexpr ParameterId WINDOW_INPUT_CURSOR_PARAMETER = "Events::Window::Input::Cursor::PARAMETER"_hash;
	constexpr ParameterId WINDOW_SET_CURSOR_PARAMETER = "Events::Window::Set::Cursor::PARAMETER"_hash;
}

#endif
 