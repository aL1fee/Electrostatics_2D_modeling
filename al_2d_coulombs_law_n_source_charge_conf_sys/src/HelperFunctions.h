#pragma once

#include <random>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm.hpp>
#include <Windows.h>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"

#include "Constants.h"

class Helper 
{
private:


public:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void turnOnWireframeMode();
	static void turnOffWireframeMode();

	static float vec3DistanceXY(glm::vec3 v);
	static float getAngle(glm::vec3 v);

	static bool checkIfVecWithinRange(float range, glm::vec3& v1, glm::vec3& v2);

	static void HideConsole() { ::ShowWindow(::GetConsoleWindow(), SW_HIDE); }

	static void ShowConsole() { ::ShowWindow(::GetConsoleWindow(), SW_SHOW); }

	static bool IsConsoleVisible() { return ::IsWindowVisible(::GetConsoleWindow()) != FALSE; }

	static void freeConsole() { FreeConsole(); }

	static void setWindow(GLFWwindow* window, int width, int height);

	static float convertWindowToNDCX(GLFWwindow* window, float x);
	static float convertWindowToNDCY(GLFWwindow* window, float y);

	static float calculateAngle(glm::vec3 v1, glm::vec3 v2);

	static float scaleVecMag(float vMag);

	static float EFieldLogMappingMag(float vMag, float vMagMin, float vMagMax);

	static double log_base(double value, double base);

	static glm::vec3 generateRandomColor();

};
