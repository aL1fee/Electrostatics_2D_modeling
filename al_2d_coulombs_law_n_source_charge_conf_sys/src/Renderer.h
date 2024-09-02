#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <random>
#include <chrono>

#include <GLFW/glfw3.h>
#include <glad/glad.h>


#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
//#include <ext.hpp>
#include <gtx/string_cast.hpp>

#include "Input.h"
#include "Shader.h"
#include "HelperFunctions.h"
#include "Texture.h"
#include "Camera.h"
#include "Constants.h"
#include "Geometry/Geometry.h"
#include "Geometry/Circle.h"
#include "Physics/Charge.h"
#include "Physics/PhysicalSystem.h"
#include "GUI/GUI.h"

class Renderer
{
private:
	Input input;
	string shaderFilepath;
	GLFWwindow* window;
	GUI* gui;
	void updateTime() const;

	void updateFrameBackgroundColors() const;


	void checkChargesForMovement(vector<Charge*>* charges, MouseLoc* cursorPos) const;


public:
	void clear() const;
	void clearDepthBuffer() const;

	void run() const;

	Renderer(const string& filepath, GLFWwindow* w, GUI* g);

	
};