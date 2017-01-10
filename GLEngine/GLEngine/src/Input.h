#pragma once
//#include "Application.h"
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//#define input (*Input::getInput())

using glm::vec2;
using glm::vec3;
using glm::vec4;

class Input
{
private:
	static Input* inputInstance;
	GLFWwindow* window;
	
	GLboolean curKeyState[1024];
	GLboolean prevKeyState[1024];

	vec2 mousePosition;

	Input(GLFWwindow* window);
	
	//keyboard callbacks
	static void staticKeyCallback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode);
	void keyCallback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode);
	
	//Mouse callbacks
	static void staticMousePosCallBack(GLFWwindow* a_window, GLdouble xPos, GLdouble yPos);
	void mousePosCallBack(GLFWwindow* a_window, GLdouble xPos, GLdouble yPos);

public:

	//returns a pointer to the input instance
	static Input* getInput();

	//creates a new instance of Input
	static void Initialize(GLFWwindow* window);


	//returns true if key is pressed
	bool getKey(GLint key);
	//returns true only on key press
	bool getKeyDown(GLint key);
	//returns true only on key release
	bool getKeyUp(GLint key);

	vec2 getMousePosition();
	void setMousePosition(vec2 position);
	

};