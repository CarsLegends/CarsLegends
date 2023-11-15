#ifndef TEXTURES_TEXTURE_HPP
#define TEXTURES_TEXTURE_HPP

#include <string>
#include <glad/glad.h>
#include <stb_image.h>

#include "../Shaders/ShaderProgram.hpp"

namespace Textures
{
	using namespace Shaders;

	class Texture
	{
	public:
		GLuint m_Id;
		const char* m_Type;
		std::string m_Path;
		GLuint m_Unit;
		Texture(const char* path, const std::string& directory, const char* textureType, GLuint slot);

		void TextureUnit(ShaderProgram& shader, const char* uniform, GLuint unit);
		void Bind();
		void Unbind();
		void Delete();
	};
}
#endif
