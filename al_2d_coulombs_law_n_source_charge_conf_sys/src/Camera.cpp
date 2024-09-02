#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 tar)
{
	cPosition = pos;
	cTarget = tar;
	cFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cDirection = glm::normalize(cPosition - cTarget);
	cUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cRight = glm::normalize(glm::cross(cUp, cDirection));
	yaw = YAW;
	pitch = PITCH;
	cDirectionYawPitch = glm::vec3(0.0f);
	zoom = 45.0f;
	sensitivity = 0.1f;
	baseSpeed = 2.5f;
}


glm::mat4 Camera::lookAt(glm::vec3 pos, glm::vec3 tar, glm::vec3 up) {
	return glm::lookAt(pos, pos + tar, up);
}

void Camera::calculateDirectionYawPitch()
{
	cDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cDirection.y = sin(glm::radians(pitch));
	cDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cFront = glm::normalize(cDirection);
}

glm::mat4 Camera::updateView() const {
	return glm::lookAt(cPosition, cPosition + cFront, cUp);
}

glm::mat4 Camera::updatePerspective() const
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return glm::perspective(glm::radians(zoom),
		(float) width / (float) height, 0.1f, 100.0f);
}
