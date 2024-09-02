#pragma once

#define GLM_ENABLE_EXPERIMENTAL 

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

#include "Charge.h"
#include "Space.h"
#include "ElectricField.h"
#include "../Input.h"

class PhysicalSystem
{
private:
	float relative_permittivity;
	std::vector<Charge*>* charges;
	std::vector<Space*>* spaces;
	bool _selectingSpace;
	
	ElectricField* efield;



	bool checkConvergingFieldlineTermination(glm::vec3 v);
	bool checkDivergingFieldlineTermination(glm::vec3 v);

public:
	PhysicalSystem(float relPerm);

	float getRelativePerm() const { return relative_permittivity; }
	void setRelativPerm(float x) { relative_permittivity = x; }

	void generateRandomConfiguration();

	std::vector<std::vector<glm::vec3>*>* buildChargeVerts();
	void freeChargeVerts(std::vector<std::vector<glm::vec3>*>* chargeVerts);

	void addCharge(Charge* ch);
	void removeCharge();
		
	void updateElectricField();
	glm::vec3 getElectricFieldAtNDC(float i, float j) const;

	void clearSystem();

	Charge* getClosestChargeToLoc(glm::vec3 v);

	bool isChargeGUIActivated();
	void GUIDeactivateAllCharges() const;
	Charge* getGUIActivatedCharge();

	void buildSystem();

	std::vector<unsigned int>* buildChargeVAOs(std::vector<std::vector<glm::vec3>*>* chargeVerts);
	void freeChargeVAOs(std::vector<unsigned int>* chargeVAOs);


	std::vector<std::vector<glm::vec3>*>* buildFieldLinesVerts(float segmentLength);
	std::vector<std::vector<glm::vec3>*>* buildFieldLinesVertsFromEField(float segmentLength);


	void freeFieldLinesVerts(std::vector<std::vector<glm::vec3>*>* fieldLinesVerts);


	bool isFieldlineAngleRejected(glm::vec3 v1, glm::vec3 v2);

	std::vector<unsigned int>* buildFieldLinesVAOs(std::vector<std::vector<glm::vec3>*>* fieldLines);
	std::vector<unsigned int>* buildFieldLinesFromEFieldVAOs(std::vector<std::vector<glm::vec3>*>* fieldLines);
	void freeFieldLinesVAOs(std::vector<unsigned int>* fieldLinesVAOs);

	void checkChargesForMovement(MouseLoc* cursorPos) const;

	void drawCharges(std::vector<std::vector<glm::vec3>*>* chargeVerts, std::vector<unsigned int>* chargeVAOs);
	void drawFieldLines(std::vector<std::vector<glm::vec3>*>* fieldLinesVerts, std::vector<unsigned int>* fieldLinesVAOs);
	void drawFieldLinesFromEField(std::vector<std::vector<glm::vec3>*>* fieldLinesVerts, std::vector<unsigned int>* fieldLinesVAOs);

	//void drawFieldLines(std::vector<std::vector<glm::vec3>*>* fieldLinesVerts, std::vector<unsigned int>* fieldLinesVAOs);

	void selectSpace();
	void deselectSpace();
	void checkSpacesForMovement(MouseLoc* cursorPos);
	std::vector<std::vector<glm::vec3>*>* buildSpaceVerts();
	std::vector<unsigned int>* buildSpaceVAOs(std::vector<std::vector<glm::vec3>*>* sps);
	void drawSpaces(std::vector<std::vector<glm::vec3>*>* spaceVerts, std::vector<unsigned int>* spaceVAOs);
	void freeSpaceVAOs(std::vector<unsigned int>* spaceVAOs);

	void deleteAllSpaces();

	void updateEColorGrid();

};