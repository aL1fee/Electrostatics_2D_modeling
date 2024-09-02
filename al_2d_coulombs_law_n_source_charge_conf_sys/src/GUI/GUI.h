#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "../Physics/PhysicalSystem.h"
#include "../Physics/Charge.h"

#include "../Constants.h"

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

//#include "../Renderer.h"

enum backgroundColor {
	GREY,
	LIGHT,
	DARK
};

class GUI
{
private:
	GLFWwindow* window;




public:


	GUI(GLFWwindow* w);

	void init();

	void newFrame(PhysicalSystem* physicalSystem);
	void render();
	void terminate();

	void showSettings(PhysicalSystem* physicalSystem);

	void buildUpperPanel(PhysicalSystem* physicalSystem);




};