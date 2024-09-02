#pragma once

#include "Geometry.h"
#include "../HelperFunctions.h"


class Circle : public Geometry {
protected:
	float radius;


public:

	Circle(float rad, int vCount, glm::vec3 pos);


	float getRadius() const { return radius; }

	std::vector<glm::vec3>* build();





};