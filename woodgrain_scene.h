#ifndef WOODGRAIN_SCENE_H
#define WOODGRAIN_SCENE_H

#include "helper/scene.h"
#include "helper/glslprogram.h"

#include <glm/glm.hpp>
class WoodGrain_Scene : public Scene
{
private:
	GLSLProgram prog;
	GLuint quad;

	glm::vec3 lightPos;
	float angle;

	void setMatrices();
	void compile();
	void drawScene();
public:
	WoodGrain_Scene();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};
#endif

