#ifndef TEXTURES_TEXTURE_HPP
#define TEXTURES_TEXTURE_HPP

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <string>
#include <glad/glad.h>

#include "../Shaders/ShaderProgram.hpp"

namespace Textures
{
	using namespace Shaders;

	class Texture
	{
	public:
		GLuint mId;
		std::string mType;
		std::string mPath;
		std::string mDirectory;
		GLuint mUnit;
		Texture();
		Texture(const char* path, const std::string& directory, const char* textureType, GLuint slot);
		Texture(FT_Face face);

		void TextureUnit(ShaderProgram& shader, const char* uniform, GLuint unit);
		void Bind();
		void Unbind();
		void Delete();
	};
}
#endif
