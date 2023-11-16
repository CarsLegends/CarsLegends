#ifndef WINDOW_MOUSE_STATE_HPP
#define WINDOW_MOUSE_STATE_HPP

#include <glm/vec2.hpp>

namespace Windows
{
	using namespace glm;

	struct MouseState
	{
		vec2 mPosition;
		bool mRightClickReleased;

		MouseState();
	};

	inline MouseState::MouseState()
	{
		this->mPosition = vec2(0.0f, 0.0f);
		this->mRightClickReleased = true;
	}
}

#endif