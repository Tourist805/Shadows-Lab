#ifndef CAMERA_H
#define CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
using glm::mat4;
using glm::vec3;



// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    void processMovementInput(GLFWwindow* window);
    mat4 getView();
    static Camera& getCamera();
    void openScene();

private:
    vec3 cameraPos = vec3(10.0f);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

};
#endif
