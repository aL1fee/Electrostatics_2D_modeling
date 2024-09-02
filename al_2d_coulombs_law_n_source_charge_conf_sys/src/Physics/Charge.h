#pragma once

#include "../Geometry/Circle.h"
#include <random>

class Charge : public Circle
{
private:
	int charge;
	//bool pressed;
	float distinctStartingLineAngle;
	std::vector<float>* fieldLineAngles;



public:
	Charge(float rad, int vCount, glm::vec3 pos, int ch);

	int getChrage() const { return charge; }
	void setCharge(int ch) { charge = ch; }

	//int numSegmentPerLine removed for the new boundary conditions on the end of lines to take place instead

	//bool isPressed() const { return pressed; }
	//void press() { pressed = true; }
	//void release() { pressed = false; }

	inline bool operator==(const Charge& otherCh) {
		return
			charge == otherCh.getChrage() &&
			radius == otherCh.getRadius() &&
			position == otherCh.getPosition();
	}

	inline bool operator!=(const Charge& otherCh) {
		return !(*this == otherCh);
	}

	float getDistinctStartingLineAngle() { return distinctStartingLineAngle; }

	void addFieldLineAngle(float f);
	std::vector<float>* getFieldLineAngles();
	void clearFieldLineAngles();
	void freeFieldLineAngles();
	bool containsAngle(float f) const;
	int getChargeAnglesSize() const;

	void removeLastAngle();

	bool hasAngleWithinDelta(float a);
	float getAngleNotWithinDelta();


};