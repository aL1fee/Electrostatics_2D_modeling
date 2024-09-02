#include "GUI.h"

GUI::GUI(GLFWwindow* w)
{
	window = w;
	//init();
}

void GUI::newFrame(PhysicalSystem* physicalSystem)
{
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    buildUpperPanel(physicalSystem);
    showSettings(physicalSystem);
}

void GUI::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::terminate()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::showSettings(PhysicalSystem* physicalSystem)
{
    ImGui::SetNextWindowPos(ImVec2(0, SIDEBAR_Y_OFFSET)); // Position at the top-left corner
    ImGui::SetNextWindowSize(ImVec2(SIDEBAR_X_OFFSET, ImGui::GetIO().DisplaySize.y)); // Full height of the screen

    ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Configuration settings");

    if (ImGui::Button("Generate a random configuration")) {
        physicalSystem->generateRandomConfiguration();
    }
    if (ImGui::Button("Deselect charges")) {
        physicalSystem->GUIDeactivateAllCharges();
    }
    if (ImGui::Button("Select a new space")) {
        physicalSystem->selectSpace();
    }
    if (ImGui::Button("Delete all spaces")) {
        physicalSystem->deleteAllSpaces();
    }
    if (ImGui::Button("Draw a Gaussian surface")) {
        physicalSystem->deleteAllSpaces();
    }
    ImGui::Text("Enter relative permittivity: ");
    float perm = physicalSystem->getRelativePerm();
    ImGui::SliderFloat(" ", &perm, 1, 11.7);
    physicalSystem->setRelativPerm(perm);

    ImGui::End();

    if (physicalSystem->isChargeGUIActivated()) {
        ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        Charge* ch = physicalSystem->getGUIActivatedCharge();
        ImGui::Text("Charge settings");
        ImGui::Text("Charge: "); 
        ImGui::SameLine();
        std::string s = std::to_string(ch->getChrage());
        char const* pchar = s.c_str();
        ImGui::Text(pchar);
        int i = ch->getChrage();
        ImGui::SliderInt("Coulomb", &i, -20, 20);   
        if (i == 0) {
            i = 1;
        }
        ch->setCharge(i);
        ImGui::Text("Charge x position: ");
        ImGui::SameLine();
        std::string xPos = std::to_string(ch->getXCenPos());
        char const* xPoschar = xPos.c_str();
        ImGui::Text(xPoschar);
        ImGui::Text("Charge y position: ");
        ImGui::SameLine();
        std::string yPos = std::to_string(ch->getYCenPos());
        char const* yPoschar = yPos.c_str();
        ImGui::Text(yPoschar);
        ImGui::Text("Enter X position: ");
        float xValue = ch->getXCenPos();
        ImGui::InputFloat("X pos", &xValue);
        ch->setPosition(glm::vec3(xValue, ch->getYCenPos(), ch->getZCenPos()));
        ImGui::Text("Enter Y position: ");
        float yValue = ch->getYCenPos();
        ImGui::InputFloat("Y pos", &yValue);
        ch->setPosition(glm::vec3(ch->getXCenPos(), yValue, ch->getZCenPos()));




        //if (ImGui::Button("Button 1")) {
        //    // Handle button click
        //}
        //ImGui::Text("Enter an integer:");
        //ImGui::InputInt("##input", &myValue);

        ImGui::End();
    }
    


}

void GUI::buildUpperPanel(PhysicalSystem* physicalSystem)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {

            if (ImGui::MenuItem("New configuration", "")) {
                physicalSystem->clearSystem();
            }

            if (ImGui::MenuItem("Open", "")) {

            }
            if (ImGui::MenuItem("Save", "")) {}
            if (ImGui::MenuItem("Exit", "")) {
                terminate();
                glfwTerminate();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}

            ImGui::Separator();
            if (ImGui::MenuItem("Remove charge", "")) {
                physicalSystem->removeCharge();
            }
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            extern bool fieldLinesOnVecsOff;
            if (ImGui::MenuItem("Toggle on/off sharp angle fieldlines")) {
                extern bool fieldlineSharpRejectionAngleOn;
                fieldlineSharpRejectionAngleOn = !fieldlineSharpRejectionAngleOn;
            }
            if (ImGui::MenuItem("Toggle on/off test charge calc unity")) {
                extern bool testChargeCalcUnity;
                testChargeCalcUnity = !testChargeCalcUnity;
            }
            if (ImGui::MenuItem("Toggle on/off line/vector mode")) {

                fieldLinesOnVecsOff = !fieldLinesOnVecsOff;
            }
            if (fieldLinesOnVecsOff == false) {
                if (ImGui::BeginMenu("Field vector settings"))
                {
                    if (ImGui::BeginMenu("Adjust vector density")) {
                        extern int vertsFromEFieldDensityOfLines;
                        ImGui::SliderInt("Line Density", &vertsFromEFieldDensityOfLines,  1, 10);
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Adjust vector line length")) {
                        extern float vectorLineLength;
                        ImGui::SliderFloat("Line length", &vectorLineLength, .003f, .04f);
                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("Toggle on/off arrows")) {
                        extern bool arrowsOn;
                        arrowsOn = !arrowsOn;
                    }



                    ImGui::EndMenu();
                }
            }
            else {
                if (ImGui::BeginMenu("Field line settings"))
                {
                    if (ImGui::BeginMenu("Adjust # of fieldlines")) {
                        extern int numberOfFieldlines;
                        ImGui::SliderInt("# of fieldlines", &numberOfFieldlines, 8, 24);
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
            }

            if (ImGui::BeginMenu("Change the background color")) {
                {
                    extern backgroundColor activeColor;
                    if (ImGui::MenuItem("Grey")) {
                        activeColor = GREY;
                    }
                    if (ImGui::MenuItem("Light")) {
                        activeColor = LIGHT;
                    }
                    if (ImGui::MenuItem("Dark")) {
                        activeColor = DARK;
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMenu();
        }





        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Add charge", "")) {
                Charge* ch = new Charge(.015f, 32, glm::vec3(-.5f, 0.2f, .0f), 2);
                physicalSystem->addCharge(ch);
            }
            if (ImGui::MenuItem("Add a space", "")) {

            }
            if (ImGui::MenuItem("Add a dipole", "")) {

            }
            if (ImGui::MenuItem("Add a capacitor", "")) {

            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("About"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}


void GUI::init()
{
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    //io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsLight();
}
