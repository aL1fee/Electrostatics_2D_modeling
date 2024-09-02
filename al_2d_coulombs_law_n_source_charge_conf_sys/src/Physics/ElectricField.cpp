#include "ElectricField.h"

void ElectricField::initializeElectricFieldGrid()
{
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			electricFieldGrid[i][j] = glm::vec3(.0f);
		}
	}
}

void ElectricField::initializeColorGrid() {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			colorGrid[i][j] = glm::vec3(.0f);
		}
	}
}

ElectricField::ElectricField()
{
	size = electricFieldGridValues::res_128;
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
	return electricFieldGrid[determineIndFromNDCX(i)][determineIndFromNDCY(j)];
}

void ElectricField::setVal(int i, int j, glm::vec3 v)
{
	electricFieldGrid[i][j] = v;
}

int ElectricField::getSize()
{
	return size;
}

float ElectricField::determineNDCX(int i) const
{
	return (2.0f / (size - 1)) * i - 1;
	
}

float ElectricField::determineNDCY(int j) const
{
	return -((2.0f / (size - 1)) * j - 1);
}

int ElectricField::determineIndFromNDCX(float x) const
{
	return (1 + x) / (2.0f / (size - 1));
}

int ElectricField::determineIndFromNDCY(float y) const
{
	return (size - 1) - (1 + y) / (2.0f / (size - 1));
}

glm::vec3 ElectricField::getColorValAtInd(int i, int j) const
{
	if (i < 0 || i > size || j < 0 || j > size) {
		return glm::vec3(.0f, .0f, .0f);
	}
	return colorGrid[i][j];
}

glm::vec3 ElectricField::getColorValAtNDC(int i, int j) const
{


	return colorGrid[determineIndFromNDCX(i)][determineIndFromNDCY(j)];
}

void ElectricField::setColorVal(int i, int j, glm::vec3 v)
{
	colorGrid[i][j] = v;
}

