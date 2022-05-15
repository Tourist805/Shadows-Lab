#include "InputController.h"

void InputController::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_1))
    {
        currentScene = HDR_SCENE;
        std::cout << "Process movement " << currentScene << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_2))
    {
        currentScene = TRANSP_SCENE;
        std::cout << "Process movement " << currentScene << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_3))
    {
        currentScene = PARTICLES_SCENE;
        std::cout << "Process movement " << currentScene << "\n";
    }
}

int InputController::getCurrentScene()
{
    return currentScene;
}

InputController& InputController::getController()
{
    static InputController* instancePtr;

    if (instancePtr == nullptr)
    {
        instancePtr = new InputController();
    }

    return *instancePtr;
}