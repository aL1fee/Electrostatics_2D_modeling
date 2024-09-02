#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Camera.h"
#include "Constants.h"
#include "HelperFunctions.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


void mouse(GLFWwindow* window, GLboolean isActive, GLboolean isClicked, GLdouble x, GLdouble y);

struct MouseLoc
{
	GLboolean isActive;
	GLboolean isClicked;
	GLboolean isSpaceActive;
	GLdouble x;
	GLdouble y;
	GLdouble xPrev;
	GLdouble yPrev;
};

class Input
{
private:
	GLFWwindow* window;




public:
	Input(GLFWwindow* w);

	void processInput() const;
	float upDownKeys(float f) const;

	void hideAndCaptureCursor() const;




};
