#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
using glm::mat4;
using glm::vec3;

enum Scenes
{
    HDR_SCENE,
    TRANSP_SCENE,
    PARTICLES_SCENE
};

class InputController 
{
private:
    int currentScene = HDR_SCENE;
public:
    int getCurrentScene();
    void processInput(GLFWwindow* window);
    static InputController& getController();
};

#endif