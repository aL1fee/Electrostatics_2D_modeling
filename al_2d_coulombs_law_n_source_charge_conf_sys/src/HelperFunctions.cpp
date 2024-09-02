#include "HelperFunctions.h"


void Helper::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//ImVec2 view = ImGui::GetContentRegionAvail();

	glViewport(SIDEBAR_X_OFFSET, 0, INITIAL_OPENGL_CONTEXT_SCREEN_WIDTH, INITIAL_OPENGL_CONTEXT_SCREEN_HEIGHT);
	glScissor(SIDEBAR_X_OFFSET, 0, INITIAL_OPENGL_CONTEXT_SCREEN_WIDTH, INITIAL_OPENGL_CONTEXT_SCREEN_HEIGHT);


	/*int widthOfViewport = 0, heightOfViewport = 0;
	int lowerLeftCornerOfViewportX = 0, lowerLeftCornerOfViewportY = 0;

	float requiredHeightOfViewport = width * (1.0f / ASPECT_RATIO);

	if (requiredHeightOfViewport > height) {
		float requiredWidthOfViewport = height * ASPECT_RATIO;
		if (requiredWidthOfViewport > width) {
			std::cout << "Error: Couldn't find dimensions that preserve the aspect ratio\n";
		}
		else {
			widthOfViewport = static_cast<int>(requiredWidthOfViewport);
			heightOfViewport = height;
			float widthOfTheTwoVerticalBars = width - widthOfViewport;
			lowerLeftCornerOfViewportX = static_cast<int>(widthOfTheTwoVerticalBars / 2.0f);
			lowerLeftCornerOfViewportY = 0;
		}
	}
	else {
		widthOfViewport = width;
		heightOfViewport = static_cast<int>(requiredHeightOfViewport);
		float heightOfTheTwoHorizontalBars = height - heightOfViewport;
		lowerLeftCornerOfViewportX = 0;
		lowerLeftCornerOfViewportY = static_cast<int>(heightOfTheTwoHorizontalBars / 2.0f);

	}

	glViewport(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, widthOfViewport, heightOfViewport);
	glScissor(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY,
		widthOfViewport, heightOfViewport);*/
}

void Helper::setWindow(GLFWwindow* window, int width, int height)
{
	framebuffer_size_callback(window, width, height);
}



void Helper::turnOnWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Helper::turnOffWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

float Helper::vec3DistanceXY(glm::vec3 v)
{
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

bool Helper::checkIfVecWithinRange(float range, glm::vec3& v1, glm::vec3& v2)
{
	if (length(v1 - v2) <= (range + 0.0f)) {
		return true;
	}
	return false;
}

float Helper::convertWindowToNDCX(GLFWwindow* window, float x)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	//std::cout << "x1: " << x << std::endl;

	int correctedWidth = width - SIDEBAR_X_OFFSET;

	float halfWidth = ((float)(width - SIDEBAR_X_OFFSET)) / 2;
	float middleX = ((float)(x - SIDEBAR_X_OFFSET)) - halfWidth;

	//std::cout << "x2: " << middleX / halfWidth << std::endl;

	return middleX / halfWidth;
	
}

float Helper::convertWindowToNDCY(GLFWwindow* window, float y)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	//std::cout << "y1: " << y << std::endl;

	int correctedHeight = height - SIDEBAR_Y_OFFSET;

	float halfHeight = ((float)(height - SIDEBAR_Y_OFFSET)) / 2;
	float middleY = ((float)(y - SIDEBAR_Y_OFFSET)) - halfHeight;

	//std::cout << "y2: " << (-middleY) / halfHeight << std::endl;

	return (-middleY) / halfHeight;
}

float Helper::calculateAngle(glm::vec3 v1, glm::vec3 v2)
{
	float dotProduct = glm::dot(v1, v2);
	return glm::degrees(glm::acos(dotProduct));
}

// mapping from 0:100000+? domian to 1:5 image
float Helper::scaleVecMag(float vMag)
{
	if ((vMag > 0) && (vMag < 5)) {
		return 0.5f;
	}
	if ((vMag > 5) && (vMag < 15)) {
		return 0.9f;
	}
	if ((vMag > 15) && (vMag < 25)) {
		return 1.2f;
	}
	if ((vMag > 25) && (vMag < 60)) {
		return 1.8f;
	}
	if ((vMag > 60) && (vMag < 120)) {
		return 2.4f;
	}
	if ((vMag > 120) && (vMag < 250)) {
		return 3.1f;
	}
	if ((vMag > 250) && (vMag < 600)) {
		return 3.7f;
	}
	if ((vMag > 600) && (vMag < 1000)) {
		return 4.3f;
	}
	if ((vMag > 1000) && (vMag < 2500)) {
		return 5.1f;
	}
	if ((vMag > 2500) && (vMag < 6000)) {
		return 5.9f;
	}
	if ((vMag > 6000) && (vMag < 10000)) {
		return 6.5f;
	}
	if ((vMag > 10000) && (vMag < 25000)) {
		return 7.3f;
	}
	if ((vMag > 25000) && (vMag < 55000)) {
		return 8.1f;
	}
	if ((vMag > 55000) && (vMag < 100000)) {
		return 9.5f;
	} 
	else {
		return 10.5f;
	}
}

double Helper::log_base(double value, double base) {
	return std::log(value) / std::log(base);
}

glm::vec3 Helper::generateRandomColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 1);
	float randomArray[3] = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < sizeof(randomArray) / sizeof(randomArray[0]); ++i) {
		randomArray[i] = distrib(gen); 
	}
	return glm::vec3(randomArray[0], randomArray[1], randomArray[2]);
}

float Helper::EFieldLogMappingMag(float vMag, float vMagMin, float vMagMax)
{
	vMagMax = 10000;
	vMagMin = 100;
	if (vMag > vMagMax) {
		vMag = vMagMax;
	}
	if (vMag < vMagMin) {
		vMag = vMagMin;
	}

	float custom_base = 2.0f;
	float log_custom_Diff = log_base(vMagMax, custom_base) - log_base(vMagMin + 1, custom_base);
	float log2Diff = log2(vMagMax) - log2(vMagMin + 1);
	//float log10Diff = log10(vMagMax) - log10(vMagMin + 1);
	float log_custom_mapping = (log_base(vMag + 1, custom_base) - log_base(vMagMin + 1, custom_base)) / log_custom_Diff;
	float log2Mapping = (log2(vMag + 1) - log2(vMagMin + 1)) / log2Diff;
	//float log10Mapping = (log(vMag + 1) - log10(vMagMin + 1)) / log10Diff;
	//float linMapping = vMag / (vMagMax - vMagMin);

	return log2Mapping;
}

//float Helper::convertWindowToNDCX(GLFWwindow* window, float x)
//{
//	int width, height;
//	glfwGetWindowSize(window, &width, &height);
//	
//	std::cout << "x1: " << x << std::endl;
//
//	int correctedWidth = width;
//
//	if (((float)width / height) >= ASPECT_RATIO) {
//		correctedWidth = height * ASPECT_RATIO;
//	}
//
//	float x1 = x - width / 2;
//	std::cout << "x2: " << (x1) / (float(correctedWidth / 2)) << std::endl;
//
//	return (x1) / (float(correctedWidth / 2));
//
//}
//
//float Helper::convertWindowToNDCY(GLFWwindow* window, float y)
//{
//	int width, height;
//	glfwGetWindowSize(window, &width, &height);
//
//	std::cout << "y1: " << y << std::endl;
//
//	int correctedHeight = height;
//
//	if (((float)width / height) < ASPECT_RATIO) {
//		correctedHeight = width * (1 / ASPECT_RATIO);
//	}
//
//	float y1 = y - height / 2;
//
//	std::cout << "y2: " << (-y1) / (float(correctedHeight / 2)) << std::endl;
//
//	return (-y1) / (float(correctedHeight / 2));
//}
