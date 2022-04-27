#ifndef SCENE_CLOUD_H
#define SCENE_CLOUD_H

#include "helper/scene.h"
#include "helper/glslprogram.h"

#include <glm/glm.hpp>

class Scene_Cloud : public Scene
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
	Scene_Cloud();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};


#endif // !SCENE_CLOUD_H