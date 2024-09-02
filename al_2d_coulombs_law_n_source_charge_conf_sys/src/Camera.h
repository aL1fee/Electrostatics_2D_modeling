#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Constants.h"

class Camera
{
private:
	GLFWwindow* window;
	glm::vec3 cPosition, cTarget, cFront, cDirection, cRight, cUp;
	float pitch;
	float yaw;
	glm::vec3 cDirectionYawPitch;
	float zoom;
	float sensitivity, baseSpeed;

public:
	
	Camera(glm::vec3 pos, glm::vec3 tar);

	glm::mat4 lookAt(glm::vec3 pos, glm::vec3 tar, glm::vec3 up);


	glm::vec3 getCameraPos() const { return cPosition; }
	glm::vec3 getCameraFront() const { return cFront; }
	glm::vec3 getCameraUp() const { return cUp; }

	float getYaw() const { return yaw; }
	float getPitch() const { return pitch; }
	float getZoom() const { return zoom; }
	float getSensistivity() const { return sensitivity; }
	float getBaseSpeed() const { return baseSpeed; }

	void setCameraPos(glm::vec3 x) { cPosition = x; }
	void setYaw(float x) { yaw = x; }
	void setPitch(float x) { pitch = x; }
	void setZoom(float x) { zoom = x; }
	
	void calculateDirectionYawPitch();

	glm::mat4 updateView() const;
	glm::mat4 updatePerspective() const;



};