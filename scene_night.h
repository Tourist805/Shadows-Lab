#ifndef SCENE_NIGHT_H
#define SCENE_NIGHT_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/plane.h"
#include "helper/torus.h"
#include "helper/teapot.h"

#include <glm/glm.hpp>

class Scene_Night : public Scene
{
private:
    GLSLProgram prog;

    GLuint fsQuad, pass1Index, pass2Index;
    GLuint renderFBO;
    GLuint renderTex;
    GLuint noiseTex;

    Plane plane;
    Torus torus;
    Teapot teapot;

    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void pass1();
    void pass2();

public:
    Scene_Night();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENENIGHTVISION_H

