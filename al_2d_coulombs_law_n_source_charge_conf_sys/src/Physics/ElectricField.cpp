#include "ElectricField.h"

void ElectricField::initializeElectricFieldGrid()
{
	for (int i = 0; i < size_AR_corr_Rows; ++i) {
		for (int j = 0; j < size_AR_corr_Cols; ++j) {
			electricFieldGrid[i][j] = glm::vec3(.0f);
		}
	}
}

void ElectricField::initializeColorGrid() {
	for (int i = 0; i < size_AR_corr_Rows; ++i) {
		for (int j = 0; j < size_AR_corr_Cols; ++j) {
			colorGrid[i][j] = glm::vec3(.0f);
		}
	}
}

ElectricField::ElectricField()
{
	size_AR_corr_Cols = FIELD_VECTOR_GRID_AR_CORR_COLS;
	size_AR_corr_Rows = FIELD_VECTOR_GRID_AR_CORR_ROWS;

	initializeElectricFieldGrid();
	initializeColorGrid();
}

glm::vec3 ElectricField::getValAtInd(int i, int j) const
{
	//std::cout << "i: " << i << std::endl;
	//std::cout << "j: " << j << std::endl;
	//std::cout << "V: " << glm::to_string(electricFieldGrid[i][j]) << std::endl;

	return electricFieldGrid[i][j];
}

glm::vec3 ElectricField::getValAtNDC(int i, int j) const
{
	// NOT USED
	//std::cout << "I: " << i << ", J: " << j << std::endl;
	return electricFieldGrid[determineIndFromNDCY(i)][determineIndFromNDCX(j)];
}

void ElectricField::setVal(int i, int j, glm::vec3 v)
{
	electricFieldGrid[i][j] = v;
}

int ElectricField::getSizeRows()
{
	return size_AR_corr_Rows;
}

int ElectricField::getSizeCols()
{
	return size_AR_corr_Cols;
}

float ElectricField::determineNDCX(int i) const
{
	return (ASPECT_RATIO * 2.0f / (size_AR_corr_Cols - 1)) * i - ASPECT_RATIO;
	
}

float ElectricField::determineNDCY(int j) const
{
	return -((2.0f / (size_AR_corr_Rows - 1)) * j - 1);
}

int ElectricField::determineIndFromNDCX(float x) const
{

	return (ASPECT_RATIO + x) / (ASPECT_RATIO * 2.0f / (size_AR_corr_Cols - 1));
}

int ElectricField::determineIndFromNDCY(float y) const
{
	return (size_AR_corr_Rows - 1) - (1 + y) / (2.0f / (size_AR_corr_Rows - 1));
}

glm::vec3 ElectricField::getColorValAtInd(int i, int j) const
{
	if (i < 0 || i > size_AR_corr_Rows || j < 0 || j > size_AR_corr_Cols) {
		return glm::vec3(.0f, .0f, .0f);
	}
	return colorGrid[i][j];
}

glm::vec3 ElectricField::getColorValAtNDC(int i, int j) const
{


	return colorGrid[determineIndFromNDCY(i)][determineIndFromNDCX(j)];
}

void ElectricField::setColorVal(int i, int j, glm::vec3 v)
{
	colorGrid[i][j] = v;
}

