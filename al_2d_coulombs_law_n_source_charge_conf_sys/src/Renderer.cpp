#include "Renderer.h"

int testNumClicks = 0;
int testNumOfLinesSegments = 0;
int testExecTimeInterval = 50;
long testExecTimeCount = 0;

bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));
float deltaTime, lastFrame = 0.0f;

float lastX = INITIAL_OPENGL_CONTEXT_SCREEN_WIDTH / 2.0f;
float lastY = INITIAL_OPENGL_CONTEXT_SCREEN_HEIGHT / 2.0f;
bool trueFPS = true;

bool testChargeCalcUnity = false;

bool fieldlineSharpRejectionAngleOn = false;

bool fieldLinesOnVecsOff = false;

backgroundColor activeColor = GREY;

int vertsFromEFieldDensityOfLines = 2;
float vectorLineLength = .009f;
bool arrowsOn = true;

int numberOfFieldlines = MIN_NUMBER_OF_LINES_PER_CHARGE;

Renderer::Renderer(const string& filepath, GLFWwindow* w, GUI* g) : input(w)
{
	shaderFilepath = filepath;
	window = w;
	gui = g;
}

void Renderer::clear() const
{
	//glClearColor function is a statesetting function and glClear is a state-using function in that it uses the current state to
	//retrieve the clearing color from.
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clearDepthBuffer() const
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::updateTime() const
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Renderer::updateFrameBackgroundColors() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	clear();
	switch (activeColor) {
		case GREY:
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			//glClearColor(0.47f, 0.47f, 0.47f, 1.0f);
			break;
		case LIGHT:
			glClearColor(0.94f, 0.94f, 0.94f, 1.0f);
			break;
		case DARK:
			glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
			break;
	}
	glEnable(GL_SCISSOR_TEST);
	clear();
	glDisable(GL_SCISSOR_TEST);
}

//void Renderer::checkChargesForMovement(vector<Charge*>* charges, MouseLoc* cursorPos) const
//{
//	//std::cout << "isActive: " << (cursorPos->isActive == GL_TRUE) << ", POS: " << cursorPos->x << ", " << cursorPos->y << std::endl;
//	//glm::vec3 cursorNormalizedPos = glm::vec3(cursorPos->x / SCREEN_WIDTH, cursorPos->y / SCREEN_HEIGHT, 0.0f);
//	//std::cout << "normalized POS: " << cursorNormalizedPos.x << ", " << cursorNormalizedPos.y << std::endl;
//
//	if (!cursorPos->isActive && cursorPos->isClicked) {
//		for (int i = 0; i < charges->size(); i++) {
//			glm::vec3 temp = charges->at(i)->getPosition();
//			glm::vec3 cursorTemp = glm::vec3(cursorPos->x, cursorPos->y, 0.0f);
//			if (Helper::checkIfVecWithinRange(charges->at(i)->getRadius(), cursorTemp, temp)) {
//				cursorPos->isActive = GL_TRUE;
//				cursorPos->isClicked = GL_FALSE;
//				charges->at(i)->press();
//				break;
//			}
//		}
//	}
//
//	if (cursorPos->isActive) {
//		//std::cout << "isActive: " << (cursorPos->isActive == GL_TRUE) << ", POS: " << cursorPos->x << ", " << cursorPos->y << std::endl;
//		//glm::vec3 cursorNormalizedPos = glm::vec3(cursorPos->x / SCREEN_WIDTH, cursorPos->y / SCREEN_HEIGHT, 0.0f);
//		for (int i = 0; i < charges->size(); i++) {
//			glm::vec3 temp = charges->at(i)->getPosition();
//			glm::vec3 cursorTemp = glm::vec3(cursorPos->x, cursorPos->y, 0.0f);
//			glm::vec3 cursorOld = glm::vec3(cursorPos->xPrev, cursorPos->yPrev, 0.0f);
//			//if (Helper::checkIfVecWithinRange(charges->at(i).getRadius(), cursorTemp, temp) || charges->at(i).isPressed()) {
//			if (charges->at(i)->isPressed()) {
//				charges->at(i)->setPosition(temp + cursorTemp - cursorOld);
//				cursorPos->xPrev = cursorPos->x;
//				cursorPos->yPrev = cursorPos->y;
//				break;
//			}
//		}
//		//std::cout << "ASADSADASD" << std::endl;
//		//cursorPos->isActive = GL_FALSE;
//	} else {
//		for (int i = 0; i < charges->size(); i++) {
//			charges->at(i)->release();
//		}
//	}
//}
//std::cout << "isActive: " << (cursorPos->isActive == GL_TRUE) << ", POS: " << cursorPos->x << ", " << cursorPos->y << std::endl;

void Renderer::run() const
{
	PhysicalSystem* physicalSystem = new PhysicalSystem(RELATIVE_PERMITTIVITY);

	Charge* ch1 = new Charge(.015f, 16, glm::vec3(-.5f, 0.0f, .0f), -3);
	Charge* ch2 = new Charge(.015f, 16, glm::vec3(0.5f, 0.0f, .0f),  3);
	//Charge* ch3 = new Charge(.12f, 32, glm::vec3(0.0f, 0.5f, .0f), 4);
	//Charge* ch4 = new Charge(.15f, 4, glm::vec3(0.1f, 0.1f, .0f), -5);
	//Charge* ch5 = new Charge(.03f, 32, glm::vec3(-.2f, 0.3f, .0f), 1);
	//Charge* ch6 = new Charge(.06f, 32, glm::vec3(-.5f, 0.2f, .0f), 2);
	//Charge* ch7 = new Charge(.06f, 4, glm::vec3(0.3f, -0.1f, .0f), -2);
	//Charge* ch8 = new Charge(.24f, 32, glm::vec3(-1.0f, -1.0f, .0f), 8);
	//Charge* ch9 = new Charge(.03f, 32, glm::vec3(-.2f, 0.3f, .0f), 1);
	//Charge* ch10 = new Charge(.06f, 32, glm::vec3(-.5f, 0.2f, .0f), 2);
	//Charge* ch11 = new Charge(.06f, 4, glm::vec3(0.3f, -0.1f, .0f), -2);
	//Charge* ch12 = new Charge(.24f, 32, glm::vec3(-1.0f, -1.0f, .0f), 8);

	physicalSystem->addCharge(ch1);
	physicalSystem->addCharge(ch2);
	//physicalSystem->addCharge(ch3);
	//physicalSystem->addCharge(ch4);
	//physicalSystem->addCharge(ch5);
	//physicalSystem->addCharge(ch6);
	//physicalSystem->addCharge(ch7);
	//physicalSystem->addCharge(ch8);
	//physicalSystem->addCharge(ch9);
	//physicalSystem->addCharge(ch10);
	//physicalSystem->addCharge(ch11);
	//physicalSystem->addCharge(ch12);

	Shader chargeShader = Shader("res/shaders/shader_circle.shader", 0);
	Shader fieldLineShader = Shader("res/shaders/shader_1_line.shader", 1);
	Shader spaceShader = Shader("res/shaders/shader_space.shader", 1);

	MouseLoc cursorPos;
	cursorPos.xPrev = 0;
	cursorPos.yPrev = 0;

	//glfwSetKeyCallback(window, key_callback);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowUserPointer(window, &cursorPos);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
	// gui init overwrites glfw callbacks if called before glfw callbacks
	gui->init();


	physicalSystem->updateElectricField();

	cursorPos.isActive = GL_FALSE;
	cursorPos.isClicked = GL_FALSE;


	while (!glfwWindowShouldClose(window)) {
		
		//glfwPollEvents();
		gui->newFrame(physicalSystem);


		auto testExecutionRenderLoopTime0 = std::chrono::high_resolution_clock::now();
		
		input.processInput();
		updateFrameBackgroundColors();
		clearDepthBuffer();

		physicalSystem->updateElectricField();
		physicalSystem->updateEColorGrid();

		physicalSystem->checkChargesForMovement(&cursorPos);
		physicalSystem->checkSpacesForMovement(&cursorPos);

		std::vector<std::vector<glm::vec3>*>* chargeVerts = physicalSystem->buildChargeVerts();
		std::vector<unsigned int>* chargeVAOs = physicalSystem->buildChargeVAOs(chargeVerts);
		chargeShader.bind();
		physicalSystem->drawCharges(chargeVerts, chargeVAOs);
		physicalSystem->freeChargeVerts(chargeVerts);
		physicalSystem->freeChargeVAOs(chargeVAOs);


		//std::vector<std::vector<glm::vec3>*>* fieldLinesVerts = physicalSystem->buildFieldLinesVerts(SEGMENT_LENGTH);
		std::vector<std::vector<glm::vec3>*>* fieldLinesVerts;
		if (fieldLinesOnVecsOff) {
			fieldLinesVerts = physicalSystem->buildFieldLinesVerts(SEGMENT_LENGTH);
		} else {
			fieldLinesVerts = physicalSystem->buildFieldLinesVertsFromEField(SEGMENT_LENGTH);
		}

		//auto testExecutionRenderLoopTime4 = std::chrono::high_resolution_clock::now();
		std::vector<unsigned int>* fieldLinesVAOs;
		if (fieldLinesOnVecsOff) {
			fieldLinesVAOs = physicalSystem->buildFieldLinesVAOs(fieldLinesVerts);
		}
		else {
			fieldLinesVAOs = physicalSystem->buildFieldLinesFromEFieldVAOs(fieldLinesVerts);
		}

		fieldLineShader.bind();

		if (fieldLinesOnVecsOff) {
			physicalSystem->drawFieldLines(fieldLinesVerts, fieldLinesVAOs);
		} else {
			physicalSystem->drawFieldLinesFromEField(fieldLinesVerts, fieldLinesVAOs);
		}
		physicalSystem->freeFieldLinesVerts(fieldLinesVerts);
		physicalSystem->freeFieldLinesVAOs(fieldLinesVAOs);

		std::vector<std::vector<glm::vec3>*>* spaceVerts = physicalSystem->buildSpaceVerts();
		std::vector<unsigned int>* spaceVAOs = physicalSystem->buildSpaceVAOs(spaceVerts);
		spaceShader.bind();
		physicalSystem->drawSpaces(spaceVerts, spaceVAOs);
		////physicalSystem->freeSpaceVerts(chargeVerts);
		physicalSystem->freeSpaceVAOs(spaceVAOs);

		//auto testExecutionRenderLoopTime5 = std::chrono::high_resolution_clock::now();
		//auto duration4 = std::chrono::duration_cast<std::chrono::microseconds> (testExecutionRenderLoopTime5 - testExecutionRenderLoopTime4);
		//std::cout << "Time test: " << duration4.count() << std::endl;


		gui->render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		//testing performance
		testExecTimeCount++;
		if (testExecTimeCount % testExecTimeInterval == 0) {
			auto testExecutionRenderLoopTime1 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds> (testExecutionRenderLoopTime1 - testExecutionRenderLoopTime0);
			std::cout << "The render loop execution time is: " << duration.count() << std::endl;
		}
	}
}