#include "ShaderProgram.hpp"

#include <glm/mat4x4.hpp>

namespace Shaders {
	using namespace std;
	string ShaderProgram::GetFileContents(const char* fileName) {
		ifstream in(fileName, ios::binary);

		if (in) {
			string contents;
			in.seekg(0, ios::end);
			contents.resize(in.tellg());
			in.seekg(0, ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return contents;
		}

		throw errno;
	}

	ShaderProgram::ShaderProgram(const char* vertexFileName, const char* fragmentFileName) {
		const string vertexCode = GetFileContents(vertexFileName);
		const string fragmentCode = GetFileContents(fragmentFileName);

		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

		const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);
		CompileErrors(vertexShader, "VERTEX");

		const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);
		CompileErrors(fragmentShader, "FRAGMENT");

		this->mId = glCreateProgram();
		glAttachShader(this->mId, vertexShader);
		glAttachShader(this->mId, fragmentShader);

		glLinkProgram(this->mId);
		CompileErrors(this->mId, "PROGRAM");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void ShaderProgram::SendUniformMatrix4f(const string& uniformName, mat4 matrix) const {
		glUniformMatrix4fv(glGetUniformLocation(this->mId, uniformName.data()), 1, GL_FALSE, value_ptr(matrix));
	}

	void ShaderProgram::Activate() const {
		glUseProgram(this->mId);
	}

	void ShaderProgram::Delete() const {
		glDeleteProgram(this->mId);
	}

	void ShaderProgram::CompileErrors(unsigned int shader, const char* type) {
		GLint hasCompiled;
		char infoLog[1024];

		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE) {
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << endl;
			}
		} else {
			glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE) {
				glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
				cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << endl;
			}
		}
	}

}
