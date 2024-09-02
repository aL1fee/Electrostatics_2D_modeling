#include "PhysicalSystem.h"


bool PhysicalSystem::checkConvergingFieldlineTermination(glm::vec3 v)
{
	// DO NOT COUNT THE CHARGE THAT YOU START WITH
	for (int i = 0; i < charges->size(); i++) {
		glm::vec3 chVec = charges->at(i)->getPosition();
		if (Helper::checkIfVecWithinRange(CONVERGING_CHARGE_TERMINATION_OPTIMIZATION, chVec, v)) {
			return true;
		}
	}
	return false;
}

bool PhysicalSystem::checkDivergingFieldlineTermination(glm::vec3 v)
{
	float lowestPos = FLOAT_MAX_LIMIT_FOR_DIVERGING_TERM_OPTIMIZATION;
	for (int i = 0; i < charges->size(); i++) {
		glm::vec3 chVec = charges->at(i)->getPosition();
		if (lowestPos > Helper::vec3DistanceXY(v - chVec)) {
			lowestPos = Helper::vec3DistanceXY(v - chVec);
		}
	}
	if (lowestPos >= DIVERGING_CHARGE_TERMINATION_OPTIMIZATION) {
		return true;
	}
	return false;
}

PhysicalSystem::PhysicalSystem(float relPerm)
{
	charges = new std::vector<Charge*>;
	spaces = new std::vector<Space*>;
	relative_permittivity = relPerm;
	_selectingSpace = false;
	efield = new ElectricField();
}



void PhysicalSystem::generateRandomConfiguration()
{
	clearSystem();

	int min = 2;
	int max = 8;
	std::random_device rd;  // Obtain a random number from hardware
	std::mt19937 gen(rd()); // Seed the generator
	std::uniform_int_distribution<> distrib(min, max); // Define the range
	int randomValue = distrib(gen);
	int* randomArray = new int[randomValue];

	for (int i = 0; i < randomValue; ++i) {
		randomArray[i] = distrib(gen); // Fill with random values in the same range
	}
	std::random_device rd2;  // Obtain a random number from hardware
	std::mt19937 gen2(rd2()); // Seed the generator
	std::uniform_real_distribution<> distrib2(-1.0, 1.0); // Define the range
	std::random_device rd3;  // Obtain a random number from hardware
	std::mt19937 gen3(rd3()); // Seed the generator
	std::uniform_int_distribution<> distrib3(-10, 10); // Define the range

	for (int i = 0; i < randomValue; i++) {
		int randomCharge = distrib3(gen3);
		if (randomCharge == 0) {
			randomCharge = -3;
		}
		std::cout << "A: " << randomCharge << std::endl;
		float randomValue1 = distrib2(gen2);
		float randomValue2 = distrib2(gen2);
		glm::vec3 v = glm::vec3(randomValue1, randomValue2, 0.0f);
		std::cout << to_string(v) << std::endl;
		Charge* ch = new Charge(.015, 32, v, randomCharge);
		addCharge(ch);
	}
	
	delete[] randomArray;

}

std::vector<std::vector<glm::vec3>*>* PhysicalSystem::buildChargeVerts()
{
	std::vector<std::vector<glm::vec3>*>* chargeVerts = new std::vector<std::vector<glm::vec3>*>;
	for (int i = 0; i < charges->size(); i++) {
		chargeVerts->push_back(charges->at(i)->build());
	}
	return chargeVerts;
}

void PhysicalSystem::freeChargeVerts(std::vector<std::vector<glm::vec3>*>* chargeVerts)
{
	for (int i = 0; i < chargeVerts->size(); i++) {
		delete chargeVerts->at(i);
	}
	delete chargeVerts;
}

void PhysicalSystem::addCharge(Charge* ch)
{
	charges->push_back(ch);
}

void PhysicalSystem::removeCharge()
{
	if (charges->size() > 0) {
		Charge* ch = charges->at(charges->size() - 1);
		ch->freeFieldLineAngles();
		charges->pop_back();
		delete ch;
	}
}

void PhysicalSystem::updateElectricField()
{
	float permittiviy_0 = PERMITTIVITY_OF_FREE_SPACE;

	for (int i = 0; i < efield->getSize(); i++) {
		for (int j = 0; j < efield->getSize(); j++) {
			glm::vec3 resultVector = glm::vec3(.0f);
			for (int c = 0; c < charges->size(); c++) {
				// test charge is always positive
				Charge* ch = charges->at(c);
				int chQ = ch->getChrage();
				glm::vec3 posV = (glm::vec3(efield->determineNDCX(i), efield->determineNDCY(j), Z_2D_COORD) - ch->getPosition());
				float pos_sq_mag = pow(Helper::vec3DistanceXY(posV), 2);
				glm::vec3 normV = glm::normalize(posV);
				resultVector += (float)chQ / pos_sq_mag * normV;
			}

			resultVector *= 1.0f / (4 * M_PI * permittiviy_0 * relative_permittivity) / VOLTS_TO_GVOLTS_PER_METER_CONV_CONST;


			efield->setVal(i, j, resultVector);
		}
	}
}

glm::vec3 PhysicalSystem::getElectricFieldAtNDC(float i, float j) const
{
	//std::cout << "i: " << i << std::endl;
	//std::cout << "j: " << j << std::endl;
	return efield->getValAtInd(efield->determineIndFromNDCX(i), efield->determineIndFromNDCY(j));
}

std::vector<std::vector<glm::vec3>*>* PhysicalSystem::buildFieldLinesVertsFromEField(float segmentLength)
{
	extern int testNumOfLinesSegments;
	extern bool fieldlineSharpRejectionAngleOn;
	extern int vertsFromEFieldDensityOfLines;
	extern float vectorLineLength;
	extern bool arrowsOn;

	std::vector<std::vector<glm::vec3>*>* ret = new std::vector<std::vector<glm::vec3>*>;

	constexpr float angle1 = glm::radians(30.0f); // Angle in radians
	constexpr float angle2 = glm::radians(-30.0f); // Angle in radians
	glm::vec3 axisOfRot(0.0f, 0.0f, 1.0f); // Axis of rotation (z-axis)
	glm::mat4 rotationMatrix1 = glm::rotate(glm::mat4(1.0f), angle1, axisOfRot);
	glm::mat4 rotationMatrix2 = glm::rotate(glm::mat4(1.0f), angle2, axisOfRot);
	float arrowHeadSize = 100.0f;

	for (int i = 0; i < efield->getSize(); i+= vertsFromEFieldDensityOfLines) {
		for (int j = 0; j < efield->getSize(); j+= vertsFromEFieldDensityOfLines) {
			bool chargeAtLineOrigin = false;
			for (int c = 0; c < charges->size(); c++) {
				float newDist = Helper::vec3DistanceXY(charges->at(c)->getPosition() -
					glm::vec3(efield->determineNDCX(i), efield->determineNDCY(j), Z_2D_COORD));
				if (newDist < CHARGE_AT_VECTOR_ORIGIN_REJECTION_VAL) {
					chargeAtLineOrigin = true;
				}
			}
			if (chargeAtLineOrigin) {
				continue;
			}
			std::vector<glm::vec3>* fieldLine = new std::vector<glm::vec3>;
			glm::vec3 pos = glm::vec3(efield->determineNDCX(i), efield->determineNDCY(j), Z_2D_COORD);
			//std::cout << "P: " << glm::to_string(pos) << std::endl;
			glm::vec3 newV = efield->getValAtInd(i, j);
			//std::cout << "newV: " << glm::to_string(glm::normalize(newV)) << std::endl;
			fieldLine->push_back(pos);
			//fieldLine->push_back(glm::vec3(.1f, .1f, .8f));
			fieldLine->push_back(efield->getColorValAtInd(i, j));
			//std::cout << "A1: " << glm::to_string(efield->getColorValAtInd(i, j)) << std::endl;

			float vec_mag = length(newV);
			float scaled_vec_mag = Helper::scaleVecMag(abs(vec_mag));
			glm::vec3 newPos = pos + glm::normalize(newV) * vectorLineLength * scaled_vec_mag;
			fieldLine->push_back(newPos);
			fieldLine->push_back(efield->getColorValAtInd(i, j));
			
			if (arrowsOn) {
				glm::vec3 arrowHeadLeft = -(newPos - pos);
				glm::vec3 arrowHeadRight = -(newPos - pos);

				glm::vec3 rotatedHeadLeft = glm::vec3(rotationMatrix1 * glm::vec4(arrowHeadLeft, 1.0f));
				glm::vec3 rotatedHeadRight = glm::vec3(rotationMatrix2 * glm::vec4(arrowHeadRight, 1.0f));

				fieldLine->push_back(newPos);
				fieldLine->push_back(efield->getColorValAtInd(i, j));
				fieldLine->push_back(newPos + glm::normalize(rotatedHeadLeft) / arrowHeadSize);
				fieldLine->push_back(efield->getColorValAtInd(i, j));
				fieldLine->push_back(newPos);
				fieldLine->push_back(efield->getColorValAtInd(i, j));
				fieldLine->push_back(newPos + glm::normalize(rotatedHeadRight) / arrowHeadSize);
				fieldLine->push_back(efield->getColorValAtInd(i, j));
			}



			//fieldLine->push_back(efield->getColorValAtNDC(newPos.x, newPos.y));
			//std::cout << "A2: " << glm::to_string(efield->getColorValAtNDC(newPos.x, newPos.y)) << std::endl;
			ret->push_back(fieldLine);
		}
	}
	//std::cout << "MAX: " << MAX_E_F << std::endl;
	//std::cout << "MIN: " << MIN_E_F << std::endl;



	//auto testExecutionRenderLoopTime0 = std::chrono::high_resolution_clock::now();
	//auto testExecutionRenderLoopTime1 = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::microseconds> (testExecutionRenderLoopTime1 - testExecutionRenderLoopTime0);
	//std::cout << "Time test: " << duration.count() << std::endl;




	//extern long testExecTimeCount;
	//extern int testExecTimeInterval;
	//if ((testExecTimeCount % testExecTimeInterval == 0) && (testExecTimeCount != 0)) {
	//	std::cout << "Num_of_segments: " << testNumOfLinesSegments++ << std::endl;
	//}
	//testNumOfLinesSegments = 0;

	return ret;
}

//std::vector<std::vector<glm::vec3>*>* PhysicalSystem::buildFieldLinesVertsFromEField(float segmentLength)
//{
//
//
//	extern int testNumOfLinesSegments;
//	extern bool fieldlineSharpRejectionAngleOn;
//	extern int vertsFromEFieldDensityOfLines;
//	extern float vectorLineLength;
//
//	std::vector<std::vector<glm::vec3>*>* ret = new std::vector<std::vector<glm::vec3>*>;
//
//
//	for (int i = 0; i < efield->getSize(); i += vertsFromEFieldDensityOfLines) {
//		for (int j = 0; j < efield->getSize(); j += vertsFromEFieldDensityOfLines) {
//			std::vector<glm::vec3>* fieldLine = new std::vector<glm::vec3>;
//			glm::vec3 pos = glm::vec3(efield->determineNDCX(i), efield->determineNDCY(j), Z_2D_COORD);
//			//std::cout << "P: " << glm::to_string(pos) << std::endl;
//			glm::vec3 newV = efield->getValAtInd(i, j);
//			//std::cout << "newV: " << glm::to_string(glm::normalize(newV)) << std::endl;
//			fieldLine->push_back(pos);
//			float vec_mag = length(newV);
//			float scaled_vec_mag = Helper::scaleVecMag(abs(vec_mag));
//			fieldLine->push_back(pos + glm::normalize(newV) * vectorLineLength * scaled_vec_mag);
//			ret->push_back(fieldLine);
//		}
//	}
//	//std::cout << "MAX: " << MAX_E_F << std::endl;
//	//std::cout << "MIN: " << MIN_E_F << std::endl;
//
//
//
//	//auto testExecutionRenderLoopTime0 = std::chrono::high_resolution_clock::now();
//	//auto testExecutionRenderLoopTime1 = std::chrono::high_resolution_clock::now();
//	//auto duration = std::chrono::duration_cast<std::chrono::microseconds> (testExecutionRenderLoopTime1 - testExecutionRenderLoopTime0);
//	//std::cout << "Time test: " << duration.count() << std::endl;
//
//
//
//
//	//extern long testExecTimeCount;
//	//extern int testExecTimeInterval;
//	//if ((testExecTimeCount % testExecTimeInterval == 0) && (testExecTimeCount != 0)) {
//	//	std::cout << "Num_of_segments: " << testNumOfLinesSegments++ << std::endl;
//	//}
//	//testNumOfLinesSegments = 0;
//
//	return ret;
//}



void PhysicalSystem::clearSystem()
{
	for (int i = 0; i < charges->size(); i++) {
		delete charges->at(i);
	}
	charges->clear();
}

Charge* PhysicalSystem::getClosestChargeToLoc(glm::vec3 v)
{
	Charge* ch = nullptr;
	float minDist = 100.0f;
	for (int i = 0; i < charges->size(); i++) {
		float x = Helper::vec3DistanceXY(v - charges->at(i)->getPosition());
		if (x < minDist) {
			minDist = x;
			ch = charges->at(i);
		}
	}
	return ch;
}

bool PhysicalSystem::isChargeGUIActivated()
{
	for (int i = 0; i < charges->size(); i++) {
		if (charges->at(i)->isGUIActivatedFunc()) {
			return true;
		}
	}
	return false;
}

void PhysicalSystem::GUIDeactivateAllCharges() const
{
	for (int i = 0; i < charges->size(); i++) {
		charges->at(i)->GUIDeactivate();
	}
}

Charge* PhysicalSystem::getGUIActivatedCharge()
{
	for (int i = 0; i < charges->size(); i++) {
		if (charges->at(i)->isGUIActivatedFunc()) {
			return charges->at(i);
		}
	}
	return nullptr;
}

void PhysicalSystem::buildSystem()
{


}

std::vector<unsigned int>* PhysicalSystem::buildChargeVAOs(std::vector<std::vector<glm::vec3>*>* chargeVerts)
{
	std::vector<unsigned int>* chargeVAOs = new std::vector<unsigned int>();
	for (int i = 0; i < charges->size(); i++) {
		unsigned int chargeVAO;
		glGenVertexArrays(1, &chargeVAO);
		glBindVertexArray(chargeVAO);
		unsigned int chargeVBO;
		glGenBuffers(1, &chargeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, chargeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * chargeVerts->at(i)->size(), &(chargeVerts->at(i)->at(0)), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		chargeVAOs->push_back(chargeVAO);
	}
	return chargeVAOs;
}

void PhysicalSystem::freeChargeVAOs(std::vector<unsigned int>* chargeVAOs)
{
	delete chargeVAOs;
}

std::vector<std::vector<glm::vec3>*>* PhysicalSystem::buildFieldLinesVerts(float segmentLength)
{
	std::vector<std::vector<glm::vec3>*>* ret = new std::vector<std::vector<glm::vec3>*>;
	extern int testNumOfLinesSegments;
	extern int numberOfFieldlines;
	extern bool fieldlineSharpRejectionAngleOn;


	for (int i = 0; i < charges->size(); i++) {
		for (int z = 0; z < charges->size(); z++) {
			charges->at(z)->clearFieldLineAngles();
		}
		Charge* currentCh = charges->at(i);
		float theta = 0.0f;
		//theta = currentCh->getDistinctStartingLineAngle();
		//int num_lines = (MIN_NUMBER_OF_LINES_PER_CHARGE + abs(currentCh->getChrage()) / 2);
		int num_lines = numberOfFieldlines;
		float deltaAngle = 360.0f / num_lines;
		float usedAngle;
		for (int j = 0; j < num_lines; j++) {
			bool skipLine = false;
			std::vector<glm::vec3>* fieldLine = new std::vector<glm::vec3>;
			fieldLine->push_back(glm::vec3(currentCh->getPosition().x, currentCh->getPosition().y, currentCh->getPosition().z));
			testNumOfLinesSegments++;
			usedAngle = theta + j * deltaAngle;

			//if (currentCh->getChargeAnglesSize()  < MIN_NUMBER_OF_LINES_PER_CHARGE) {
			//	usedAngle = theta + j * deltaAngle;
			//	//std::cout << "A: " << usedAngle << std::endl;
			//	if (currentCh->hasAngleWithinDelta(usedAngle)) {
			//		usedAngle = currentCh->getAngleNotWithinDelta();
			//	}
			//}
			//else {
			//	break;
			//}

			currentCh->addFieldLineAngle(usedAngle);
			glm::vec3 segment(currentCh->getPosition().x + cos(glm::radians(usedAngle)) * segmentLength,
				currentCh->getPosition().y + sin(glm::radians(usedAngle)) * segmentLength,
				currentCh->getPosition().z);

			//std::cout << usedAngle << std::endl;
			//float diff = glm::degrees(atan2(glm::normalize((segment - currentCh->getPosition())).y - glm::vec3(1.0, 0.0f, .0f).y,
			//	glm::normalize((segment - currentCh->getPosition())).x - glm::vec3(1.0, 0.0f, .0f).x));
			//std::cout << "A " << diff  << std::endl;
			
			
			//float dotProduct = glm::dot(segment - currentCh->getPosition(), glm::vec3(1.0, .0f, .0f));
			//float actualAngle = glm::degrees(glm::acos(dotProduct));
			//std::cout << "A " << actualAngle << std::endl;

			extern bool testChargeCalcUnity;

			fieldLine->push_back(segment);
			testNumOfLinesSegments++;
			for (int k = 0; k < FIELDLINE_NUMBER_OF_SEGMENTS; k++) {
				glm::vec3 resultVector = glm::vec3(.0f);
				for (int l = 0; l < charges->size(); l++) {
					Charge* otherCharge = charges->at(l);
					if (fieldlineSharpRejectionAngleOn) {
						if (k == 0) {
							if ((currentCh->getChrage() <= 1 && otherCharge->getChrage() <= 1) ||
								(currentCh->getChrage() >= 1 && otherCharge->getChrage() >= 1)) {
								if (isFieldlineAngleRejected(glm::normalize(segment - currentCh->getPosition()),
									glm::normalize(otherCharge->getPosition() - currentCh->getPosition()))) {
									skipLine = true;
								}
							}
						}
					}

					int polarity = currentCh->getChrage();
					//test charge is 1 if positive, going to negative source; negative otherwise
					//if (testChargeCalcUnity) {
					//	if (polarity >= 1) {
					//		polarity = 100;
					//	}
					//	else {
					//		polarity = -100;
					//	}
					//}
					glm::vec3 r2r0 = segment - glm::vec3(otherCharge->getXCenPos(), otherCharge->getYCenPos(), otherCharge->getZCenPos());
					float r2r0_size_sq = pow(Helper::vec3DistanceXY(r2r0), 2);
					resultVector += (otherCharge->getChrage() * polarity / r2r0_size_sq) * glm::normalize(r2r0);
				}
				//float mag = Helper::vec3DistanceXY(resultVector);
				//std::cout << "H: " << glm::to_string(magnitudeVec) << std::endl;
				float newAngleDeg = atan2(resultVector.y, resultVector.x);

				//if (atan2(segment.y, segment.x) == newAngleDeg) {
				//	std::cout << "SA: " << newAngleDeg << std::endl;
				//	skipLine = true;
				//}

				if (skipLine) {
					currentCh->removeLastAngle();
					break;
				}

				//segmentLength = glm::length(resultVector);
				//std::cout << segmentLength << std::endl;
				segment = segment + glm::vec3(cos(newAngleDeg) * segmentLength, sin(newAngleDeg) * segmentLength, currentCh->getZCenPos());

				if (checkConvergingFieldlineTermination(segment)) {
					glm::vec3 normalizedOutVec = -glm::normalize(glm::vec3(cos(newAngleDeg) * segmentLength, sin(newAngleDeg) * segmentLength,
						currentCh->getZCenPos()));
					float convToChargeAngle = Helper::calculateAngle(normalizedOutVec, glm::vec3(1.0f, .0f, .0f));
					Charge* convToCharge = getClosestChargeToLoc(segment);
					convToCharge->addFieldLineAngle(convToChargeAngle);
					break;
				}
				if (checkDivergingFieldlineTermination(segment)) {
					break;
				}

				fieldLine->push_back(segment);
				testNumOfLinesSegments++;
			}
			if (!skipLine) {
				ret->push_back(fieldLine);
			}
		}
	}
	extern long testExecTimeCount;
	extern int testExecTimeInterval;
	if ((testExecTimeCount % testExecTimeInterval == 0) && (testExecTimeCount != 0)) {
		std::cout << "Num_of_segments: " << testNumOfLinesSegments++ << std::endl;
	}
	testNumOfLinesSegments = 0;

	return ret;
}

bool PhysicalSystem::isFieldlineAngleRejected(glm::vec3 v1, glm::vec3 v2)
{
	float dotProduct = glm::dot(v1, v2);
	float actualAngle = glm::degrees(glm::acos(dotProduct));
	if (actualAngle < FIELDLINE_REJECTION_ANGLE) {
		return true;
	}
	return false;
}


void PhysicalSystem::freeFieldLinesVerts(std::vector<std::vector<glm::vec3>*>* fieldLinesVerts)
{
	for (int i = 0; i < fieldLinesVerts->size(); i++) {
		delete fieldLinesVerts->at(i);
	}
	delete fieldLinesVerts;
}

std::vector<unsigned int>* PhysicalSystem::buildFieldLinesVAOs(std::vector<std::vector<glm::vec3>*>* fieldLines)
{
	std::vector<unsigned int>* fieldLinesVAOs = new std::vector<unsigned int>();
	for (int i = 0; i < fieldLines->size(); i++) {
		unsigned int fieldLinesVAO;
		glGenVertexArrays(1, &fieldLinesVAO);
		glBindVertexArray(fieldLinesVAO);
		unsigned int fieldLinesVBO;
		glGenBuffers(1, &fieldLinesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, fieldLinesVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (*fieldLines)[i]->size(), &(*(*fieldLines)[i])[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		fieldLinesVAOs->push_back(fieldLinesVAO);
	}
	return fieldLinesVAOs;
}

// redo: do useless work in copying arrays
std::vector<unsigned int>* PhysicalSystem::buildFieldLinesFromEFieldVAOs(std::vector<std::vector<glm::vec3>*>* fieldLines)
{
	std::vector<unsigned int>* fieldLinesVAOs = new std::vector<unsigned int>();

	std::vector<glm::vec3> lines = std::vector<glm::vec3>();

	for (int i = 0; i < fieldLines->size(); i++) {
		lines.insert(lines.end(), fieldLines->at(i)->begin(), fieldLines->at(i)->end());
	}

	unsigned int fieldLinesVAO;
	glGenVertexArrays(1, &fieldLinesVAO);
	glBindVertexArray(fieldLinesVAO);
	unsigned int fieldLinesVBO;
	glGenBuffers(1, &fieldLinesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fieldLinesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lines.size(), lines.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//std::cout << "SA" << std::endl;
	fieldLinesVAOs->push_back(fieldLinesVAO);

	return fieldLinesVAOs;
}

//// redo: do useless work in copying arrays
//std::vector<unsigned int>* PhysicalSystem::buildFieldLinesFromEFieldVAOs(std::vector<std::vector<glm::vec3>*>* fieldLines)
//{
//	std::vector<unsigned int>* fieldLinesVAOs = new std::vector<unsigned int>();
//
//	std::vector<glm::vec3> lines = std::vector<glm::vec3>();
//
//	for (int i = 0; i < fieldLines->size(); i++) {
//		lines.insert(lines.end(), fieldLines->at(i)->begin(), fieldLines->at(i)->end());
//	}
//
//	unsigned int fieldLinesVAO;
//	glGenVertexArrays(1, &fieldLinesVAO);
//	glBindVertexArray(fieldLinesVAO);
//	unsigned int fieldLinesVBO;
//	glGenBuffers(1, &fieldLinesVBO);
//	glBindBuffer(GL_ARRAY_BUFFER, fieldLinesVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lines.size(), lines.data(), GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	fieldLinesVAOs->push_back(fieldLinesVAO);
//
//	return fieldLinesVAOs;
//}

void PhysicalSystem::freeFieldLinesVAOs(std::vector<unsigned int>* fieldLinesVAOs)
{
	delete fieldLinesVAOs;
}

void PhysicalSystem::checkChargesForMovement(MouseLoc* cursorPos) const
{

	if (!cursorPos->isActive && cursorPos->isClicked) {

		std::cout << "A: " << glm::to_string(getElectricFieldAtNDC(cursorPos->x, cursorPos->y)) << std::endl;

		for (int i = 0; i < charges->size(); i++) {
			glm::vec3 temp = charges->at(i)->getPosition();
			glm::vec3 cursorTemp = glm::vec3(cursorPos->x, cursorPos->y, 0.0f);
			if (Helper::checkIfVecWithinRange(charges->at(i)->getRadius(), cursorTemp, temp)) {
				cursorPos->isActive = GL_TRUE;
				cursorPos->isClicked = GL_FALSE;
				GUIDeactivateAllCharges();
				charges->at(i)->press();
				charges->at(i)->GUIActivate();
				break;
			}
		}
	}
	if (cursorPos->isActive) {
		//std::cout << "isActive: " << (cursorPos->isActive == GL_TRUE) << ", POS: " << cursorPos->x << ", " << cursorPos->y << std::endl;
		//glm::vec3 cursorNormalizedPos = glm::vec3(cursorPos->x / SCREEN_WIDTH, cursorPos->y / SCREEN_HEIGHT, 0.0f);
		for (int i = 0; i < charges->size(); i++) {
			glm::vec3 temp = charges->at(i)->getPosition();
			glm::vec3 cursorTemp = glm::vec3(cursorPos->x, cursorPos->y, 0.0f);
			glm::vec3 cursorOld = glm::vec3(cursorPos->xPrev, cursorPos->yPrev, 0.0f);
			//if (Helper::checkIfVecWithinRange(charges->at(i).getRadius(), cursorTemp, temp) || charges->at(i).isPressed()) {
			if (charges->at(i)->isPressed()) {
				charges->at(i)->setPosition(temp + cursorTemp - cursorOld);
				cursorPos->xPrev = cursorPos->x;
				cursorPos->yPrev = cursorPos->y;
				break;
			}
		}
		//cursorPos->isActive = GL_FALSE;
	}
	else {
		for (int i = 0; i < charges->size(); i++) {
			charges->at(i)->release();
		}
	}
}

void PhysicalSystem::drawCharges(std::vector<std::vector<glm::vec3>*>* chargeVerts, std::vector<unsigned int>* chargeVAOs)
{
	for (int i = 0; i < chargeVerts->size(); i++) {
		glBindVertexArray(chargeVAOs->at(i));
		glDrawArrays(GL_LINE_STRIP, 0, chargeVerts->at(i)->size());
	}
}

void PhysicalSystem::drawFieldLines(std::vector<std::vector<glm::vec3>*>* fieldLinesVerts, std::vector<unsigned int>* fieldLinesVAOs)
{
	for (int i = 0; i < fieldLinesVerts->size(); i++) {
		glBindVertexArray(fieldLinesVAOs->at(i));
		glDrawArrays(GL_LINE_STRIP, 0, fieldLinesVerts->at(i)->size());
	}
}


void PhysicalSystem::drawFieldLinesFromEField(std::vector<std::vector<glm::vec3>*>* fieldLinesVerts, std::vector<unsigned int>* fieldLinesVAOs)
{
	std::vector<glm::vec3> linesVerts = std::vector<glm::vec3>();


	for (int i = 0; i < fieldLinesVerts->size(); i++) {
		linesVerts.insert(linesVerts.end(), fieldLinesVerts->at(i)->begin(), fieldLinesVerts->at(i)->end());
	}
	
	//std::cout << "X1: " << linesVerts.size() << std::endl;
	//std::cout << "X2: " << fieldLinesVAOs->size() << std::endl;


	glBindVertexArray(fieldLinesVAOs->at(0));
	glDrawArrays(GL_LINES, 0, linesVerts.size());


	//for (int i = 0; i < 1; i++) {
	//	glBindVertexArray(fieldLinesVAOs->at(i));
	//	glDrawArrays(GL_LINE_STRIP, 0, fieldLinesVAOs->size());
	//}
}

void PhysicalSystem::selectSpace()
{
	_selectingSpace = true;
}

void PhysicalSystem::deselectSpace()
{
	_selectingSpace = false;
}

void PhysicalSystem::checkSpacesForMovement(MouseLoc* cursorPos)
{
	for (int i = 0; i < charges->size(); i++) {
		if (charges->at(i)->isPressed()) {
			deselectSpace();
			return;
		}
	}
	if (_selectingSpace) {
		cursorPos->isSpaceActive = GL_TRUE;
		if (!cursorPos->isActive && cursorPos->isClicked) {
			//std::cout << "A: " << std::endl;
			cursorPos->isActive = GL_TRUE;
			deselectSpace();
			Space* s = new Space(1.0, glm::vec3(.4, .4, .4), glm::vec3(cursorPos->x, cursorPos->y, 0.0f));
			spaces->push_back(s);
		}
	}
	if (cursorPos->isActive) {
		//std::cout << "isActive: " << (cursorPos->isActive == GL_TRUE) << ", POS: " << cursorPos->x << ", " << cursorPos->y << std::endl;
		//glm::vec3 cursorNormalizedPos = glm::vec3(cursorPos->x / SCREEN_WIDTH, cursorPos->y / SCREEN_HEIGHT, 0.0f);
		spaces->at(spaces->size() - 1)->createNewVert(glm::vec3(cursorPos->x, cursorPos->y, 0.0f));
		//std::cout << "A: " << std::endl;
	}

	//	for (int i = 0; i < charges->size(); i++) {
	//		glm::vec3 temp = charges->at(i)->getPosition();
	//		glm::vec3 cursorTemp = glm::vec3(cursorPos->x, cursorPos->y, 0.0f);
	//		glm::vec3 cursorOld = glm::vec3(cursorPos->xPrev, cursorPos->yPrev, 0.0f);
	//		//if (Helper::checkIfVecWithinRange(charges->at(i).getRadius(), cursorTemp, temp) || charges->at(i).isPressed()) {
	//		if (charges->at(i)->isPressed()) {
	//			charges->at(i)->setPosition(temp + cursorTemp - cursorOld);
	//			cursorPos->xPrev = cursorPos->x;
	//			cursorPos->yPrev = cursorPos->y;
	//			break;
	//		}
	//	}
	//	//cursorPos->isActive = GL_FALSE;
	//}
	//else {
	//	for (int i = 0; i < charges->size(); i++) {
	//		charges->at(i)->release();
	//	}
	//}


}

std::vector<std::vector<glm::vec3>*>* PhysicalSystem::buildSpaceVerts()
{
	std::vector<std::vector<glm::vec3>*>* spaceVerts = new std::vector<std::vector<glm::vec3>*>;
	for (int i = 0; i < spaces->size(); i++) {
		spaceVerts->push_back(spaces->at(i)->build());
	}
	return spaceVerts;
}

std::vector<unsigned int>* PhysicalSystem::buildSpaceVAOs(std::vector<std::vector<glm::vec3>*>* sps)
{
	std::vector<unsigned int>* spaceVAOs = new std::vector<unsigned int>();
	for (int i = 0; i < sps->size(); i++) {
		unsigned int spaceVAO;
		glGenVertexArrays(1, &spaceVAO);
		glBindVertexArray(spaceVAO);
		unsigned int spaceVBO;
		glGenBuffers(1, &spaceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, spaceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (*sps)[i]->size(), &(*(*sps)[i])[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		spaceVAOs->push_back(spaceVAO);
	}
	return spaceVAOs;
}

void PhysicalSystem::drawSpaces(std::vector<std::vector<glm::vec3>*>* spaceVerts, std::vector<unsigned int>* spaceVAOs)
{
	for (int i = 0; i < spaceVerts->size(); i++) {
		glBindVertexArray(spaceVAOs->at(i));
		glDrawArrays(GL_LINE_STRIP, 0, spaceVerts->at(i)->size());
	}
}

void PhysicalSystem::freeSpaceVAOs(std::vector<unsigned int>* spaceVAOs)
{
	delete spaceVAOs;
}

void PhysicalSystem::deleteAllSpaces()
{
	for (int i = 0; i < spaces->size(); i++) {
		std::cout << "spaces size " << spaces->size() << std::endl;
		std::cout << "zzzzz" << spaces->size() << std::endl;
		spaces->at(i)->freeSpace();
		delete spaces->at(i);
	}
	spaces->clear();
	std::cout << "A: " << spaces->size() << std::endl;
}

void PhysicalSystem::updateEColorGrid()
{
	glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);

	float maxE = -100.0f;
	float minE = 10000.0f;

	for (int i = 0; i < efield->getSize(); i++) {
		for (int j = 0; j < efield->getSize(); j++) {
			float eFieldMag = (glm::length(efield->getValAtInd(i, j)));
			if (eFieldMag < minE) {
				minE = eFieldMag;
			}
			else if (eFieldMag > maxE) {
				maxE = eFieldMag;
			}
		}
	}

	float minMaxDiff = maxE - minE;
	//std::cout << "MAX: " << maxE << std::endl;
	//std::cout << "MIN: " << minE << std::endl;


	if (charges->size() != 0) {
		for (int i = 0; i < efield->getSize(); i++) {
			for (int j = 0; j < efield->getSize(); j++) {
				Charge* closestCh = charges->at(0);
				float minDistToClosestCh = 100.0f;
				float totalEfieldMag = Helper::vec3DistanceXY(efield->getValAtInd(i, j));
				glm::vec3 finalColor = glm::vec3(0.0f, 0.0f, 0.0f);
				for (int c = 0; c < charges->size(); c++) {
					Charge* ch = charges->at(c);
					int chQ = ch->getChrage();
					glm::vec3 posV = (glm::vec3(efield->determineNDCX(i), efield->determineNDCY(j), Z_2D_COORD) - ch->getPosition());
					float pos_sq_mag = pow(Helper::vec3DistanceXY(posV), 2);
					glm::vec3 normV = glm::normalize(posV);
					glm::vec3 fieldDueToC = (float)chQ / pos_sq_mag * normV * 1.0f 
						/ (float) (4 * M_PI * PERMITTIVITY_OF_FREE_SPACE * relative_permittivity) / VOLTS_TO_GVOLTS_PER_METER_CONV_CONST;
					if (chQ < 0) {
						finalColor += blue * Helper::vec3DistanceXY(fieldDueToC) / totalEfieldMag;
					}
					else {
						finalColor += red * Helper::vec3DistanceXY(fieldDueToC) / totalEfieldMag;
					}
				}

				efield->setColorVal(i, j, finalColor);

				//for (int c = 0; c < charges->size(); c++) {
				//	float newDist = Helper::vec3DistanceXY(charges->at(c)->getPosition() - glm::vec3(efield->determineNDCX(i), efield->determineNDCY(j), Z_2D_COORD));
				//	if (newDist < minDistToClosestCh) {
				//		closestCh = charges->at(c);
				//		minDistToClosestCh = newDist;
				//	}
				//}

				////float eFieldNormMag = ((glm::length(efield->getValAtInd(i, j)) - minE)) / minMaxDiff;
				//float eFieldNormMag = Helper::EFieldLogMappingMag(glm::length(efield->getValAtInd(i, j)), minE, maxE);
				////std::cout << "MAG: " << eFieldNormMag << std::endl;

				//if (closestCh->getChrage() <= 0) {
				//	efield->setColorVal(i, j, glm::vec3(0.0f, 0.0f, eFieldNormMag));
				//}
				//else {
				//	efield->setColorVal(i, j, glm::vec3(eFieldNormMag, 0.0f, 0.0f));
				//}

				//if (closestCh->getChrage() <= 0) {
				//	efield->setColorVal(i, j, glm::vec3(eFieldNormMag, 0.0f, 1.0f - eFieldNormMag));
				//}
				//else {
				//	efield->setColorVal(i, j, glm::vec3(1.0f - eFieldNormMag, 0.0f, eFieldNormMag));
				//}
			}
		}
	}
}
