#ifndef WINDOW_CONTROLLER_AXLES_HPP
#define WINDOW_CONTROLLER_AXLES_HPP

#include "../../../Configurations/WindowConfiguration.hpp"

namespace Windows
{
	using namespace Configuration;

	const enum class Axes : int
	{
		LeftX,
		LeftY,
		RightX,
		RightY,
		LeftTrigger,
		RightTrigger
	};

	struct ControllerState
	{
		float mLeftJoystickX;
		float mLeftJoystickY;
		float mRightJoystickX;
		float mRightJoystickY;
		float mLeftTrigger;
		float mRightTrigger;
		int mPlayerNumber;
		std::bitset<WINDOW_CONTROLLER_BUTTONS_COUNT> mControllerButtons;

		ControllerState();
	};

	inline ControllerState::ControllerState()
	{
		this->mLeftJoystickX = 0;
		this->mLeftJoystickY = 0;
		this->mRightJoystickX = 0;
		this->mRightJoystickY = 0;
		this->mLeftTrigger = 0;
		this->mRightTrigger = 0;
		this->mPlayerNumber = 0;
		mControllerButtons = {};
	}
}

#endif