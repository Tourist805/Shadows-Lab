#ifndef SCENE_TRANSPARENCY_H
#define SCENE_TRANSPARENCY_H

#include "helper/scene.h"
#include "helper/glslprogram.h"
#include "helper/cube.h"
#include "helper/sphere.h"
#include "camera.h"

#include <glm/glm.hpp>

enum BufferNames {
	COUNTER_BUFFER = 0,
	LINKED_LIST_BUFFER
};

struct ListNode
{
	glm::vec4 colour;
	GLfloat depth;
	GLuint next;
};

class Scene_Transparency : public Scene
{
private:
	GLSLProgram prog;

	GLuint buffers[2], fsQuad, headPtrTex, pass1Index, pass2Index, clearBuffer;

	Cube cube;
	Camera camera;
	Sphere sphere;

	float angle, tPrev, rotSpeed;

	void setMatrices();
	void compile();
	void drawScene();
	void initShaderStorage();
	void pass1();
	void pass2();
	void clearBuffers();

public:
	Scene_Transparency();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};
#endif
