#include "GUIController.h"

void GUIController::Initialize(GLFWwindow* window)
{
	GUIController::window = window;
}

void GUIController::ObserveChanges()
{
	if (window == nullptr)
	{
		return;
	}
	else if (!isInit)
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 430");
		ImGui::StyleColorsDark();

		isInit = true;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Console");

	ImGui::SliderFloat("Material rougness", &materialInfo.roughness, 0.0f, 5.0f, "%2f", 1);

	ImGui::Checkbox("Metalic: ", &materialInfo.IsMetal);

	float col[3] = { materialInfo.Colour.x, materialInfo.Colour.y, materialInfo.Colour.z };
	if (ImGui::ColorPicker3("Wave Color", col))
	{
		materialInfo.Colour = glm::vec3(col[0], col[1], col[2]);
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

MaterialInfo GUIController::getMaterial()
{
	return materialInfo;
}