#pragma once

#include <iostream>
#include <vector>

#include "glm.hpp"
#include <gtx/string_cast.hpp>
#include "../Constants.h"

class Space
{
private:
	float relative_permittivity;
	glm::vec3 color;
	std::vector<glm::vec3>* spaceVerts;
	glm::vec3 firstLocation;

public:

	Space(float rp, glm::vec3 col, glm::vec3 fL);

	void createNewVert(glm::vec3 v);

	bool checkIfWithinSpace();

	std::vector<glm::vec3>* build();

	void freeSpace();

};