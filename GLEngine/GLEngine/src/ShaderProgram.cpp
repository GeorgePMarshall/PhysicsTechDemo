#include "ShaderProgram.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>


ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(programID);
}


GLuint ShaderProgram::CreateShader(const char* name, GLenum shaderType)
{
	std::string path("shaders/"); 
	path += name;
	std::string shaderSource;
	std::ostringstream shaderStream;
	std::ifstream shaderFile(path);
	shaderStream << shaderFile.rdbuf();
	shaderSource = shaderStream.str();
	const GLchar* shaderCharSource = shaderSource.c_str();
	shaderFile.close();

	//Vertex Shader creation
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCharSource, 0);
	glCompileShader(shader);
	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(shader, infoLogLength, 0, infoLog);
		
		switch (shaderType)
		{
		case(GL_VERTEX_SHADER):
			std::cout << "Vertex shader ";
			break;
		case(GL_GEOMETRY_SHADER) :
			std::cout << "Geometry shader ";
			break;
		case(GL_FRAGMENT_SHADER) :
			std::cout << "Fragment shader ";
			break;
		default:
			std::cout << "Unknown shader ";
			break;
		}

		std::cout << "\""<< path << "\" creation failed: \n" << infoLog << '\n';

		delete[] infoLog;
	}

	return shader;
}

void ShaderProgram::LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	GLint programSuccess = GL_FALSE;
	glGetProgramiv(programID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		std::cout << "Failed to link shader program: \n" << infoLog << '\n';

		delete[] infoLog;
	}
}
void ShaderProgram::LinkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
{
	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, geometryShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	GLint programSuccess = GL_FALSE;
	glGetProgramiv(programID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		std::cout << "Failed to link shader program: \n" << infoLog << '\n';

		delete[] infoLog;
	}
}

void ShaderProgram::CreateShaderProgram(const char* vertexShaderPath, const char** varyings, GLuint amount)
{
	if (programID != 0)
	{
		glDeleteProgram(programID);
		programID = 0;
	}

	GLuint vertexShader = CreateShader(vertexShaderPath, GL_VERTEX_SHADER);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	if (amount != 0)
		glTransformFeedbackVaryings(programID, amount, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(programID);

	GLint programSuccess = GL_FALSE;
	glGetProgramiv(programID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		std::cout << "Failed to link shader program: \n" << infoLog << '\n';

		delete[] infoLog;
	}

	glDeleteShader(vertexShader);

}

void ShaderProgram::CreateShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	if (programID != 0)
	{
		glDeleteProgram(programID);
		programID = 0;
	}

	GLuint vertexShader = CreateShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = CreateShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	LinkProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void ShaderProgram::CreateShaderProgram(const char* vertexShaderPath, const char* geometryShaderPath, const char* fragmentShaderPath)
{
	if (programID != 0)
	{
		glDeleteProgram(programID);
		programID = 0;
	}

	GLuint vertexShader = CreateShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint geometryShader = CreateShader(geometryShaderPath, GL_GEOMETRY_SHADER);
	GLuint fragmentShader = CreateShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	LinkProgram(vertexShader, geometryShader,fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

void ShaderProgram::useProgram()
{
	glUseProgram(programID);
}

void ShaderProgram::setInt(const char* name, GLint value)
{
	useProgram();
	glUniform1i(glGetUniformLocation(programID, name), value);
}
void ShaderProgram::setFloat(const char* name, GLfloat value)
{
	useProgram();
	glUniform1f(glGetUniformLocation(programID, name), value);
}
void ShaderProgram::setVec2(const char* name, const glm::vec2 &value)
{
	useProgram();
	glUniform2f(glGetUniformLocation(programID, name), value.x, value.y);
}
void ShaderProgram::setVec3(const char* name, const glm::vec3 &value)
{
	useProgram();
	glUniform3f(glGetUniformLocation(programID, name), value.x, value.y, value.z);
}
void ShaderProgram::setVec4(const char* name, const glm::vec4 &value)
{
	useProgram();
	glUniform4f(glGetUniformLocation(programID, name), value.x, value.y, value.z, value.w);
}
void ShaderProgram::setMat4(const char* name, const glm::mat4 &value)
{
	useProgram();
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, glm::value_ptr(value));
}
void ShaderProgram::setFloatArray(const char* name, const float* value, GLuint amount)
{
	useProgram();
	glUniformMatrix4fv(glGetUniformLocation(programID, name), amount, GL_FALSE, value);
}


