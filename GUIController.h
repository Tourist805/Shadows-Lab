#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>

struct MaterialInfo
{
	float roughness = 0.25f;
	bool IsMetal = false;
	glm::vec3 Colour;
};

class GUIController
{
public:
	void Initialize(GLFWwindow* window);
	void ObserveChanges();

	MaterialInfo materialInfo;
	MaterialInfo getMaterial();
private:
	GLFWwindow* window;
	bool isInit = false;
};


