#include "Space.h"

Space::Space(float rp, glm::vec3 col, glm::vec3 fL)
{
	relative_permittivity = rp;
	color = col;
	spaceVerts = new std::vector<glm::vec3>();
	firstLocation = fL;
}

void Space::createNewVert(glm::vec3 v)
{
	spaceVerts->push_back(v);
}

std::vector<glm::vec3>* Space::build()
{
	return spaceVerts;
}

void Space::freeSpace()
{
	delete spaceVerts;
}

