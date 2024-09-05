#include "Input.h"

int mouseDragOldState = GLFW_RELEASE;

Input::Input(GLFWwindow* w)
{
	window = w;
}

void Input::processInput() const
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	extern Camera camera;
	extern float deltaTime;
	extern bool trueFPS;

	const float cameraSpeed = camera.getBaseSpeed() * deltaTime; // adjust accordingly
	
	glm::vec3 step = cameraSpeed * camera.getCameraFront();
	if (trueFPS) {
		step = step * glm::vec3(1.0f, 0.0f, 1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		extern int testNumClicks;
		testNumClicks++;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.setCameraPos(camera.getCameraPos() + step);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.setCameraPos(camera.getCameraPos() - step);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.setCameraPos(camera.getCameraPos() - glm::normalize(glm::cross(camera.getCameraFront(),
			camera.getCameraUp())) * cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.setCameraPos(camera.getCameraPos() + glm::normalize(glm::cross(camera.getCameraFront(),
			camera.getCameraUp())) * cameraSpeed);
	}


}

float Input::upDownKeys(float f) const
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && f <= .995) {
		std::cout << f << std::endl;
		f += .005;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && f >= .005) {
		std::cout << f << std::endl;
		f -= .005;
	}

	return f;
}

void Input::hideAndCaptureCursor() const
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	extern bool firstMouse;
	extern float lastX, lastY;
	extern Camera camera;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	xoffset *= camera.getSensistivity();
	yoffset *= camera.getSensistivity();

	camera.setYaw(camera.getYaw() + xoffset);
	camera.setPitch(camera.getPitch() + yoffset);

	if (camera.getPitch() > 89.0f)
		camera.setPitch(89.0f);
	if (camera.getPitch() < -89.0f)
		camera.setPitch(-89.0f);
	
	camera.calculateDirectionYawPitch();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	extern Camera camera;
	camera.setZoom(camera.getZoom() - (float) yoffset);
	if (camera.getZoom() < 1.0f)
		camera.setZoom(1.0f);
	if (camera.getZoom() > 45.0f)
		camera.setZoom(45.0f);
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	//if (ImGui::GetCurrentContext() != nullptr) {
	//	ImGuiIO& io = ImGui::GetIO();
	//	if (button >= 0 && button < IM_ARRAYSIZE(io.MouseDown)) {
	//		io.MouseDown[button] = (action == GLFW_PRESS);
	//	}
	//}


	MouseLoc* dataPtr = (MouseLoc*)glfwGetWindowUserPointer(window);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		dataPtr->isActive = GL_FALSE;
		dataPtr->isClicked = GL_FALSE;
		dataPtr->isSpaceActive = GL_FALSE;
		mouseDragOldState = GLFW_RELEASE;
		return;
	}
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)) 
	{
		if (mouseDragOldState == GLFW_RELEASE) {


			mouse(window, GL_FALSE, GL_TRUE, xpos, ypos);
			mouseDragOldState = GLFW_PRESS;
			dataPtr->xPrev = Helper::convertWindowToNDCX(window, xpos);
			dataPtr->yPrev = Helper::convertWindowToNDCY(window, ypos);

			std::cout << "A: " << Helper::convertWindowToNDCX(window, xpos) << ", B: " << Helper::convertWindowToNDCY(window, ypos) << std::endl;
		}
		else {
			mouse(window, dataPtr->isActive, GL_FALSE, xpos, ypos);
		}
	}
}


void mouse(GLFWwindow* window, GLboolean isActive, GLboolean isClicked, GLdouble x, GLdouble y)
{
	MouseLoc* dataPtr = (MouseLoc*) glfwGetWindowUserPointer(window);
	dataPtr->isActive = isActive;
	dataPtr->isClicked = isClicked;
	dataPtr->x = Helper::convertWindowToNDCX(window, x);
	dataPtr->y = Helper::convertWindowToNDCY(window, y);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//if (ImGui::GetCurrentContext() != nullptr) {
	//	ImGuiIO& io = ImGui::GetIO();
	//	if (button >= 0 && button < IM_ARRAYSIZE(io.MouseDown)) {
	//		io.MouseDown[button] = (action == GLFW_PRESS);
	//	}
	//}
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{

	}

}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{

}
