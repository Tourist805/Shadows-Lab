#ifndef SCENE_PBR_H
#define SCENE_PBR_H


#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/plane.h"
#include "helper/torus.h"
#include "helper/teapot.h"
#include "helper/objmesh.h"
#include "helper/noisetex.h"
#include "GUIController.h"

#include <glm/glm.hpp>
class Scene_Pbr : public Scene
{
private:
	GLSLProgram prog;

	Plane plane;
	std::unique_ptr<ObjMesh> mesh;
	Teapot teapot;
	GUIController* UIcontroller;
	float tPrev, lightAngle, lightRotationSpeed;
	glm::vec4 lightPos;

	void setMatrices();
	void compile();
	void drawScene();
	void drawFloor();
	void drawSpot(const glm::vec3& pos, float rough, int metal, const glm::vec3& colour);
public:
	Scene_Pbr();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};

#endif