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
	ret->push_back(glm::vec3(position.x, position.y * 1.0f, position.z));
	//ret->push_back(glm::vec3(position.x, position.y * 1.0f * ASPECT_RATIO, position.z));

	for (int i = 0; i < numPoints; i++) {
		float currAngle = angle * i;
		float x = position.x + radius * cos(glm::radians(currAngle));
		float y = position.y + radius * sin(glm::radians(currAngle));
		//float y = position.y + radius * sin(glm::radians(currAngle)) * ASPECT_RATIO;

		float z = position.z + 0.0f;
		ret->push_back(glm::vec3(x, y, z));
	}
	glm::vec3 initial(position.x + radius, position.y * 1.0f, position.z);
	//glm::vec3 initial(position.x + radius, position.y * 1.0f * ASPECT_RATIO, position.z);

	ret->push_back(initial);

	//std::cout << "Hey" << std::endl;
	//for (int i = 0; i < ret->size(); i++) {
	//	std::cout << glm::to_string(ret->at(i))  << std::endl;
	//}


	return ret;
}

//std::vector<glm::vec3>* Circle::build()
//{
//	float angle = 360.0f / numPoints;
//	std::vector<glm::vec3>* ret = new std::vector<glm::vec3>;
//	for (int i = 0; i < numPoints; i++) {
//		float currAngle = angle * i;
//		float x = position.x + radius * cos(glm::radians(currAngle));
//		float y = position.y + radius * sin(glm::radians(currAngle)) * ASPECT_RATIO;
//		float z = position.z + 0.0f;
//		ret->push_back(glm::vec3(x, y, z));
//	}
//	glm::vec3 initial(position.x + radius, position.y * 1.0f * ASPECT_RATIO, position.z);
//	ret->push_back(initial);
//	return ret;
//}

