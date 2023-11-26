#ifndef SHADERS_SHADER_PROGRAM_H
#define SHADERS_SHADER_PROGRAM_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cerrno>

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Shaders
{
	using namespace glm;

	class ShaderProgram
	{
	public:
		GLuint mId;

		ShaderProgram(const char* vertexFileName, const char* fragmentFileName);
		void SendUniformMatrix4f(const char* uniformName, mat4& matrix) const;
		void SendUniformVector4f(const char* uniformName, vec4& vector) const;
		void Activate() const;
		void Delete() const;

	private:
		static std::string GetFileContents(const char* fileName);
		void CompileErrors(unsigned int shader, const char* type);
	};
}
#endif
