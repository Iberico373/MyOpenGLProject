#pragma once

#include <iostream>
#include <GLAD/gl.h>

namespace ShaderManager
{
	GLuint CompileShader(unsigned int type, const std::string& source)
	{
		GLuint id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		// Error handling
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			// Get error message
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);

			// Print error message
			std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
			std::cout << message << std::endl;

			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	// Compiles both 'vertexShader' and 'fragmentShader' into a single shader program
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint shaderProgram = glCreateProgram();
		GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(shaderProgram, vs);
		glAttachShader(shaderProgram, fs);
		glLinkProgram(shaderProgram);
		glValidateProgram(shaderProgram);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return shaderProgram;
	}
};

