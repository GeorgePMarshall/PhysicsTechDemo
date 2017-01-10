#pragma once
#include "Application.h"


class ShaderProgram
{
private:
	GLuint programID = 0;
	
	GLuint CreateShader(const char* path, GLenum shaderType);
	void LinkProgram(GLuint vertexShader, GLuint fragmentShader);
	void LinkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);


public:
	void CreateShaderProgram(const char* vertexShaderPath, const char** varyings = nullptr, GLuint amount = 0);
	void CreateShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
	void CreateShaderProgram(const char* vertexShaderPath, const char* geometryShaderPath, const char* fragmentShaderPath);

	void useProgram();

	void setInt(const char* name, GLint value);
	void setFloat(const char* name, GLfloat value);
	void setVec2(const char* name, const glm::vec2 &value);
	void setVec3(const char* name, const glm::vec3 &value);
	void setVec4(const char* name, const glm::vec4 &value);
	void setMat4(const char* name, const glm::mat4 &value);
	void setFloatArray(const char* name, const float* value, GLuint amount);

	~ShaderProgram();
};