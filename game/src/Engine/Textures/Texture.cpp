#include <iostream>
#include <ostream>
#include "Texture.hpp"

namespace Textures
{
	Texture::Texture(const char* path, const std::string& directory, const char* textureType, GLuint slot)
	{
		this->m_Type = textureType;
		this->m_Path = std::string(path);

		auto filename = std::string(path);
		filename = directory + '/' + filename;

		int widthImg, heightImg, numColCh;

		// Flips the path so it appears right side up
		//stbi_set_flip_vertically_on_load(true);

		unsigned char* bytes = stbi_load(filename.c_str(), &widthImg, &heightImg, &numColCh, 0);

		if (bytes)
		{
			GLenum format;
			if (numColCh == 1)
			{
				format = GL_RED;
			}
			else if (numColCh == 3)
			{
				format = GL_RGB;
			}
			else
			{
				format = GL_RGBA;
			}

			glGenTextures(1, &m_Id);
			glActiveTexture(GL_TEXTURE0 + slot);
			this->m_Unit = slot;
			glBindTexture(GL_TEXTURE_2D, m_Id);

			glTexImage2D(GL_TEXTURE_2D, 0, format, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			stbi_image_free(bytes);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(bytes);
		}
	}

	void Texture::TextureUnit(ShaderProgram& shader, const char* uniform, GLuint unit)
	{
		GLuint textureUniform = glGetUniformLocation(shader.mId, uniform);
		shader.Activate();
		glUniform1i(textureUniform, unit);
	}

	void Texture::Bind()
	{
		glActiveTexture(GL_TEXTURE0 + this->m_Unit);
		glBindTexture(GL_TEXTURE_2D, this->m_Id);
	}

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Delete()
	{
		glDeleteTextures(1, &this->m_Id);
	}
}
