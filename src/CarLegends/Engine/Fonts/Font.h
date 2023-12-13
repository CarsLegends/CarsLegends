#ifndef FONTS_FONT_HPP
#define FONTS_FONT_HPP

#include <map>

#include "Structs/Character.h"
#include "../Textures/Texture.hpp"
#include "../../Configurations/WindowConfiguration.hpp"

namespace Fonts
{
	using namespace Textures;
	using namespace Shaders;
	using namespace Configuration;

	class Font
	{
	public:
		Font();
		Font(int fontSize);
		std::map<char, Character> mCharacters;
	};
}

#endif