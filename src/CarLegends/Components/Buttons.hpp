#ifndef COMPONENTS_BUTTONS_HPP
#define COMPONENTS_BUTTONS_HPP

#include <array>
#include <cassert>
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../Configurations/GUIConfiguration.hpp"
#include "../Configurations/WindowConfiguration.hpp"

namespace Components
{
	using namespace Configuration;
	using namespace Events;
	using namespace glm;

	struct Button
	{
		float mWidth;
		float mHeight;
		vec2 mPosition;
		vec3 mColor;
		EventId mEvent;
		std::string mLabel;
		int mFontSize = 48;
		bool mHovered;

		Button();
		Button(vec2 position, vec3 color, float width, float height, const std::string& label, EventId eventId, bool hovered = false);

		Button& CenterHorizontally();
		Button& CenterVertically();
		Button& Move(vec2 distance);
		Button& SetFontSize(int fontSize);
	};

	inline Button::Button()
	{
		this->mPosition = { 0.0f, 0.0f };
		this->mColor = { 0.0f, 0.0f, 0.0f };
		this->mWidth = 0.0f;
		this->mHeight = 0.0f;
		this->mLabel = "";
		this->mHovered = false;
	}

	inline Button& Button::CenterHorizontally()
	{
		this->mPosition.x = WINDOW_WIDTH * 0.5f;
		return *this;
	}

	inline Button& Button::CenterVertically()
	{
		this->mPosition.y = WINDOW_HEIGHT * 0.5f;
		return *this;
	}

	inline Button& Button::Move(vec2 distance)
	{
		this->mPosition.y += distance.y;
		this->mPosition.x += distance.x;
		return *this;
	}

	inline Button& Button::SetFontSize(int fontSize)
	{
		this->mFontSize = fontSize;
		return *this;
	}

	inline Button::Button(vec2 position, vec3 color, float width, float height, const std::string& label, EventId eventId, bool hovered)
	{
		this->mPosition = position;
		this->mColor = color;
		this->mWidth = width;
		this->mHeight = height;
		this->mLabel = label;
		this->mEvent = eventId;
		this->mHovered = hovered;
	}

	struct Buttons
	{
		std::array<Button, MAX_BUTTONS> mButtons;
		unsigned int mRegisteredButtonsCount = 0;

		Buttons();
		explicit Buttons(const std::vector<Button>& buttons);
	};

	inline Buttons::Buttons() = default;

	inline Buttons::Buttons(const std::vector<Button>& buttons)
	{
		assert(buttons.size() <= MAX_BUTTONS && "Max of buttons exceed");

		for (int i = 0; i < buttons.size(); i++)
		{
			this->mButtons[i] = buttons[i];
			this->mRegisteredButtonsCount++;
		}
	}
}

#endif
