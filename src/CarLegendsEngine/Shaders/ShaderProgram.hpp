#ifndef SHADER_SHADER_PROGRAM_H
#define SHADER_SHADER_PROGRAM_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cerrno>

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Shaders {
	using namespace std;
	using namespace glm;
	class ShaderProgram {
	public:
		GLuint mId;

		ShaderProgram(const char* vertexFileName, const char* fragmentFileName);
		void SendUniformMatrix4f(const string& uniformName, mat4 matrix) const;
		void Activate() const;
		void Delete() const;
	private:
		static string GetFileContents(const char* fileName);
		void CompileErrors(unsigned int shader, const char* type);
	};
}
#endif