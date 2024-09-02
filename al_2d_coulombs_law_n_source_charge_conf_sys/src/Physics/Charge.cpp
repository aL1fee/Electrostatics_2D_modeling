#include "Charge.h"

Charge::Charge(float rad, int vCount, glm::vec3 pos, int ch) : Circle(rad, vCount, pos)
{
	charge = ch;
	pressed = false;
	active = false;
	isGUIActivated = false;
	fieldLineAngles = new std::vector<float>();


	std::random_device rd;  // Obtain a random number from hardware
	std::mt19937 gen(rd()); // Seed the generator
	std::uniform_int_distribution<> distrib(0, 360); // Define the range
	float randomValue = float(distrib(gen));
	distinctStartingLineAngle = randomValue;

}

void Charge::addFieldLineAngle(float f)
{
	fieldLineAngles->push_back(f);
}

std::vector<float>* Charge::getFieldLineAngles()
{
	return fieldLineAngles;
}

void Charge::clearFieldLineAngles()
{
	fieldLineAngles->clear();
}

void Charge::freeFieldLineAngles()
{
	delete fieldLineAngles;
}

bool Charge::containsAngle(float f) const
{
	if (std::count(fieldLineAngles->begin(), fieldLineAngles->end(), f) > 0) {
		return true;
	}
	return false;
}

int Charge::getChargeAnglesSize() const
{
	return fieldLineAngles->size();
}

void Charge::removeLastAngle()
{
	fieldLineAngles->pop_back();
}

bool Charge::hasAngleWithinDelta(float a)
{
	for (int i = 0; i < fieldLineAngles->size(); i++) {
		if ((fieldLineAngles->at(i) + CONVERGING_CHARGE_ANGLE_DELTA > a) && (fieldLineAngles->at(i) - CONVERGING_CHARGE_ANGLE_DELTA < a)) {
			return true;
		}
	}
	return false;
}

float Charge::getAngleNotWithinDelta()
{
	int x = 0;
	for (int i = 0; i < MIN_NUMBER_OF_LINES_PER_CHARGE; i++) {
		if (!hasAngleWithinDelta(x + i * (360 / MIN_NUMBER_OF_LINES_PER_CHARGE))) {
			return x + i * (360 / MIN_NUMBER_OF_LINES_PER_CHARGE);
		}
	}
	return 0.0f;

}
