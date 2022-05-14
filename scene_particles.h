#ifndef SCENE_PARTICLES
#define SCENE_PARTICLES

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/texture.h"
#include <glm/glm.hpp>

class Scene_Particles : public Scene
{
private:
    GLSLProgram renderProg, computeProg;

    glm::ivec3 nParticles;
    GLuint totalParticles;

    float time, deltaT, speed, angle;
    GLuint particlesVao;
    GLuint bhVao, bhBuf;  // black hole VAO and buffer
    glm::vec4 bh1, bh2, bh3;

    void compile();
    void initBuffers();
    void setMatrices();

public:
    Scene_Particles();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};
# endif