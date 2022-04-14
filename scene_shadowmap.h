#ifndef SCENE_SHADOWMAP_H
#define SCENE_SHADOWMAP_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/plane.h"
#include "helper/torus.h"
#include "helper/teapot.h"
#include "helper/frustum.h"

#include <glm/glm.hpp>

class Scene_Shadowmap : public Scene
{
private:
	GLSLProgram prog, solidProg;
	GLuint shadowFBO, pass1Index, pass2Index;

	Teapot teapot;
	Plane plane;
	Torus torus;

	int shadowMapWidth, shadowMapHeight;
	float tPrev, angle;
	
	glm::mat4 lightPV, shadowBias;

	Frustum lightFrustum;

	void setMatrices();
	void compile();
	void setupFBO();
	void drawScene();
	void spitOutDepthBuffer();
public:
	Scene_Shadowmap();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};
#endif // !SCENE_SHADOWMAP



