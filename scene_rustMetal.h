#ifndef SCENE_RUSTMETAL_H
#define SCENE_RUSTMETAL_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/teapot.h"
#include "helper/skybox.h"
#include "helper/texture.h"
#include "helper/noisetex.h"

#include <glm/glm.hpp>

class Scene_RustMetal : public Scene
{
private:
	GLSLProgram prog, skyProg;

	Teapot teapot;
	SkyBox sky;

	float angle, tPrev, rotSpeed;

	void setMatrices(GLSLProgram &);
	void compile();
public:
	Scene_RustMetal();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};
#endif

