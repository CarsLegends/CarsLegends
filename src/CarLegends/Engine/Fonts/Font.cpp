#include <ft2build.h>
#include FT_FREETYPE_H  

#include "Font.h"

#include <iostream>

namespace Fonts
{
    Font::Font() = default;

	Font::Font(int fontSize)
    {
        FT_Library ftLibrary;
        if (FT_Init_FreeType(&ftLibrary))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            std::abort();
        }

        FT_Face face;
        if (FT_New_Face(ftLibrary, "./Resources/Fonts/game_over.ttf", 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            std::abort();
        }

        // Move to configuration

        FT_Set_Pixel_Sizes(face, WINDOW_FONT_WIDTH, fontSize);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            const Texture texture(face);
            Character character(texture, face);
            mCharacters.insert(std::pair<char, Character>(c, character));
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        FT_Done_Face(face);
        FT_Done_FreeType(ftLibrary);
    }
}
