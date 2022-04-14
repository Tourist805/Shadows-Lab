#ifndef SCENE_PCF_H
#define SCENE_PCF_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/frustum.h"

#include <glm/glm.hpp>

class Scene_Pcf : public Scene
{
private:
    GLSLProgram prog;
    GLuint shadowFBO, pass1Index, pass2Index;

    Plane plane;
    std::unique_ptr<ObjMesh> mesh;

    Frustum lightFrustum;

    int shadowMapWidth, shadowMapHeight;
    float tPrev;

    glm::mat4 lightPV;
    glm::mat4 shadowScale;
    glm::vec3 lightPos;
    float angle;

    void setMatrices();
    void compile();
    void setupFBO();
    void drawBuildingScene();

public:
    Scene_Pcf();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEPCF_H

