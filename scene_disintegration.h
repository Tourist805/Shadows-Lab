#ifndef SCENE_DISINTEGRATION_H
#define SCENE_DISINTEGRATION_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/teapot.h"

#include <glm/glm.hpp>
class Scene_Disintegration : public Scene
{
private:
	GLSLProgram prog;
	GLuint quad;

	Teapot teapot;
	glm::vec3 lightPos;
	float angle;

	void setMatrices();
	void compile();
	void drawScene();
public:
	Scene_Disintegration();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};

#endif

