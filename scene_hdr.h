#ifndef HDR_H
#define HDR_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/cube.h"
#include "helper/plane.h"
#include "helper/teapot.h"
#include "helper/sphere.h"
#include "helper/objmesh.h"
#include "helper/torus.h"
#include "camera.h"
#include "helper/noisetex.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
class Scene_Hdr : public Scene
{
private:
    GLSLProgram prog, rustProg;

    GLFWwindow* window;
    GLuint fsQuad;
    GLuint hdrFbo, blurFbo;
    GLuint hdrTex, tex1, tex2;
    GLuint linearSampler, nearestSampler;
    Torus torus;
    //std::unique_ptr<ObjMesh> spot;
    
    // Camera
    float deltaTime = 0.0f;
    Camera camera;
    float lastX = (float)width / 2.0;
    float lastY = (float)height / 2.0;
    bool firstMouse = true;

    void processInput();

    Plane plane;
    Cube cube;
    Teapot teapot;
    Sphere sphere;

    float angle, tPrev, rotSpeed;
    int bloomBufWidth, bloomBufHeight;

    void setMatrices(GLSLProgram& p);
    void compile();
    void setupFBO();
    void pass1();
    void pass2();
    void pass3();
    void pass4();
    void pass5();
    float gauss(float, float);
    void drawScene();
    void computeLogAveLuminance();

public:
    Scene_Hdr();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};
#endif

