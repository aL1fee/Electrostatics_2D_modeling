#pragma once

#include <iostream>
#include <vector>
#include <random>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

#include "Charge.h"
#include "Space.h"
#include "../Input.h"

class ElectricField
{
private:
	//enum electricFieldGridValues {
	//	res_32 = 32,
	//	res_64 = 64,
	//	res_128 = 128,
	//	res_256 = 256
	//};

	glm::vec3 electricFieldGrid[FIELD_VECTOR_GRID_AR_CORR_ROWS][FIELD_VECTOR_GRID_AR_CORR_COLS];
	glm::vec3 colorGrid[FIELD_VECTOR_GRID_AR_CORR_ROWS][FIELD_VECTOR_GRID_AR_CORR_COLS];

	int size_AR_corr_Cols;
	int size_AR_corr_Rows;

	void initializeElectricFieldGrid();
	void initializeColorGrid();

public:
	
	ElectricField();

	glm::vec3 getValAtInd(int i, int j) const;
	glm::vec3 getValAtNDC(int i, int j) const;

	void setVal(int i, int j, glm::vec3 v);
	int getSizeRows();
	int getSizeCols();

	float determineNDCX(int i) const;
	float determineNDCY(int j) const;

	int determineIndFromNDCX(float x) const;
	int determineIndFromNDCY(float y) const;

	glm::vec3 getColorValAtInd(int i, int j) const;
	glm::vec3 getColorValAtNDC(int i, int j) const;

	void setColorVal(int i, int j, glm::vec3 v);



};