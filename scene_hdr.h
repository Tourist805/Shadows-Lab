#ifndef HDR_H
#define HDR_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/cube.h"
#include "helper/plane.h"
#include "helper/teapot.h"
#include "helper/sphere.h"

#include <glm/glm.hpp>
class Scene_Hdr : public Scene
{
private:
    GLSLProgram prog;

    GLuint fsQuad;
    GLuint hdrFbo, blurFbo;
    GLuint hdrTex, tex1, tex2;
    GLuint linearSampler, nearestSampler;

    Plane plane;
    Teapot teapot;
    Sphere sphere;

    float angle, tPrev, rotSpeed;
    int bloomBufWidth, bloomBufHeight;

    void setMatrices();
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

