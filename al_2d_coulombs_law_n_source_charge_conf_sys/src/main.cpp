#include <iostream>

//GLAD manages function pointers for OpenGL for the application cross-system compatibility?
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "HelperFunctions.h"
#include "Renderer.h"
#include "GUI/GUI.h"
//#include "Shader.h"
//#include "Windows.h"
//#include <tchar.h>
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"

using namespace std;

//int WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window;
	window = glfwCreateWindow(INITIAL_GLFW_WINDOW_WIDTH, INITIAL_GLFW_WINDOW_HEIGHT, "Electrostatics Colulomb's law", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	} 

	//Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D
	//coordinates it processed to coordinates on your screen
	glViewport(0, 0, INITIAL_OPENGL_CONTEXT_SCREEN_WIDTH, INITIAL_OPENGL_CONTEXT_SCREEN_HEIGHT);
	

	//There are many callbacks functions we can set to register our own functions.For example, we
	//can make a callback function to process joystick input changes, process error messages etc.We
	//register the callback functions after we’ve created the window and before the render loop is initiated.
	
	Helper::framebuffer_size_callback(window, INITIAL_GLFW_WINDOW_WIDTH, INITIAL_GLFW_WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, Helper::framebuffer_size_callback);


	GUI* gui = new GUI(window);
	Renderer renderer("NILres/shaders/shader_1.shader", window, gui);





	//Helper::HideConsole();
	//Helper::freeConsole();





	renderer.run();


	glfwTerminate();
	gui->terminate();

	return 0;
}