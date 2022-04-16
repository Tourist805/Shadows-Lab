#ifndef SCENE_SOFT_H
#define SCENE_SOFT_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/frustum.h"

#include <glm/glm.hpp>

class Scene_Soft : public Scene
{
private:
	GLSLProgram prog;
	GLuint shadowFBO, pass1Index, pass2Index;

	Plane plane;
	std::unique_ptr<ObjMesh> mesh;

	Frustum lightFrustum;

	int samplesU, samplesV, jitterMapSize, shadowMapWidth, shadowMapHeight;
	float radius, angle;

	glm::mat4 lightPV, shadowScale;
	glm::vec3 lightPos;

	void setMatrices();
	void compile();
	void setupFBO();
	float jitter();
	void buildJitterTex();
	void drawBuildingScene();
public:
	Scene_Soft();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};
#endif

