#ifndef FONTS_STRUCT_CHARACTER_HPP
#define FONTS_STRUCT_CHARACTER_HPP

#include <glm/vec2.hpp>
#include "../Textures/Texture.hpp"

namespace Fonts
{
    using namespace Textures;

    struct Character
	{
        Texture mTexture;
        ivec2 mSize;
        ivec2 mBearing;
        unsigned int mAdvance;

        Character();
        Character(const Texture& texture, FT_Face face);
    };

    inline Character::Character() = default;

    inline Character::Character(const Texture& texture, FT_Face face)
    {
        this->mTexture = texture;
        this->mSize = ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        this->mBearing = ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        this->mAdvance = face->glyph->advance.x;
    }
}

#endif