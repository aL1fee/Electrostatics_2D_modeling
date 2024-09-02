#include "Geometry.h"

//std::vector<glm::vec3>* Geometry::buildCircle(float radius, int vCount, float xPos, float yPos, float zPos)
//{
//	float angle = 360.0f / vCount;
//	std::vector<glm::vec3>* ret = new std::vector<glm::vec3>;
//	float normalization = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;
//	for (int i = 0; i < vCount; i++) {
//		float currAngle = angle * i;
//		float x = xPos + radius * cos(glm::radians(currAngle));
//		float y = yPos + radius * sin(glm::radians(currAngle)) * normalization;
//		float z = zPos + 0.0f;
//		ret->push_back(glm::vec3(x, y, z));
//	}
//	glm::vec3 initial(xPos + radius * 1.0f, yPos, zPos);
//	ret->push_back(initial);
//	return ret;
//}
//
//std::vector<glm::vec3>* Geometry::buildFieldLinesFromTwoCircles(std::vector<glm::vec3>* circle1, std::vector<glm::vec3>* circle2, int numLines)
//{
//	std::vector<glm::vec3>* ret = new std::vector<glm::vec3>;
//	return nullptr;
//}
