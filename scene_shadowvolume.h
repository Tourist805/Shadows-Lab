#ifndef SCENE_SHADOWVOLUME_H
#define SCENE_SHADOWVOLUME_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/plane.h"
#include "helper/objmesh.h"

#include <glm/glm.hpp>

class Scene_ShadowVolume : public Scene
{
private:
	GLSLProgram volumeProg, renderProg, compProg;
	GLuint colourDepthFBO, fsQuad, spotTex, brickTex;

	Plane plane;
	std::unique_ptr<ObjMesh> spot;

	glm::vec4 lightPos;
	float angle, tPrev, rotSpeed;

	void setMatrices(GLSLProgram&);
	void compile();
	void setupFBO();
	void drawScene(GLSLProgram&, bool);
	void pass1();
	void pass2();
	void pass3();
	void updateLight();

public:
	Scene_ShadowVolume();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};
#endif
