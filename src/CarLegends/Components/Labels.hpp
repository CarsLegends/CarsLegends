#ifndef COMPONENTS_LABELS_HPP
#define COMPONENTS_LABELS_HPP

#include <array>
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../Configurations/GUIConfiguration.hpp"
#include "../Configurations/WindowConfiguration.hpp"

namespace Components
{
	using namespace glm;
	using namespace Configuration;

	struct Label
	{
		std::string mLabel;
		vec2 mPosition;
		int mFontSize = 48;
		vec3 mColor;
		bool mScreenCentered;

		Label();
		Label(const std::string& label, vec3 color, int fontSize = 48, bool centered = false);
		Label& CenterHorizontally();
		Label& CenterVertically();
		Label& Move(vec2 distance);
	};

	inline Label::Label() = default;

	inline Label::Label(const std::string& label, vec3 color, int fontSize, bool centered)
	{
		this->mLabel = label;
		this->mColor = color;
		this->mFontSize = fontSize;
		this->mPosition = vec2 { 0.0f };
		this->mScreenCentered = centered;
	}

	inline Label& Label::CenterHorizontally()
	{
		this->mPosition.x = WINDOW_WIDTH * 0.5f;
		return *this;
	}

	inline Label& Label::CenterVertically()
	{
		this->mPosition.y = WINDOW_HEIGHT * 0.5f;
		return *this;
	}

	inline Label& Label::Move(vec2 distance)
	{
		this->mPosition.y += distance.y;
		this->mPosition.x += distance.x;
		return *this;
	}

	struct Labels
	{
		std::array<Label, MAX_LABELS> mLabels;
		unsigned int mRegisteredLabelsCount = 0;

		Labels();
		explicit Labels(const std::vector<Label>& labels);
	};

	inline Labels::Labels() = default;

	inline Labels::Labels(const std::vector<Label>& labels)
	{
		assert(labels.size() <= MAX_LABELS && "Max of labels exceed");

		for (int i = 0; i < labels.size(); i++)
		{
			this->mLabels[i] = labels[i];
			this->mRegisteredLabelsCount++;
		}
	}
}

#endif