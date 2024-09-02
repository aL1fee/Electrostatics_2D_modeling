#include "Circle.h"

Circle::Circle(float rad, int vCount, glm::vec3 pos)
{
	position = pos;
	radius = rad;
	numPoints = vCount;
	isGUIActivated = false;
}

std::vector<glm::vec3>* Circle::build()
{
	float angle = 360.0f / numPoints;
	std::vector<glm::vec3>* ret = new std::vector<glm::vec3>;
	for (int i = 0; i < numPoints; i++) {
		float currAngle = angle * i;
		float x = position.x + radius * cos(glm::radians(currAngle)) / ASPECT_RATIO;
		float y = position.y + radius * sin(glm::radians(currAngle));
		float z = position.z + 0.0f;
		ret->push_back(glm::vec3(x, y, z));
	}
	glm::vec3 initial(position.x + radius * 1.0f / ASPECT_RATIO, position.y, position.z);
	ret->push_back(initial);
	return ret;
}

