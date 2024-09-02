#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib> // For rand() and srand()
#include <ctime>
#include <limits>

#include <stdio.h>
#include <math.h>

#include "glm.hpp"
#include <gtx/string_cast.hpp>
#include "../Constants.h"

class Geometry
{
protected:
	glm::vec3 position;

	int numPoints;

	bool pressed;
	bool active;
	bool isGUIActivated;



public:

	virtual std::vector<glm::vec3>* build() = 0;


	glm::vec3 getPosition() const { return position; }

	float getXCenPos() const { return position.x; }
	float getYCenPos() const { return  position.y; }
	float getZCenPos() const { return  position.z; }
	int getNumPoints() const { return numPoints; }

	void setPosition(glm::vec3 v) { position = v; }

	bool isPressed() const { return pressed; }
	void press() { pressed = true; }
	void release() { pressed = false; }

	bool isActive() const { return active; }
	void activate() { active = true; }
	void deactive() { active = false; }

	bool isGUIActivatedFunc() const { return isGUIActivated; }
	void GUIActivate() { isGUIActivated = true; }
	void GUIDeactivate() { isGUIActivated = false; }



};