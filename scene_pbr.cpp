#include "scene_pbr.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Scene_Pbr::Scene_Pbr() : plane(20, 20, 1, 1), teapot(5, glm::mat4(1.0f)), tPrev(0.0f), lightPos(5.0f, 5.0f, 5.0f, 1.0f) {
	mesh = ObjMesh::load("../Shadows-Lab/media/tinker.obj");
}

void Scene_Pbr::initScene() {
	compile();

	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(
		glm::vec3(0.0f, 3.0f, 6.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	projection = glm::perspective(glm::radians(50.0f), (float)width / height, 0.5f, 100.0f);

	lightAngle = 0.0f;
	lightRotationSpeed = 1.5f;

	prog.setUniform("Light[0].L", glm::vec3(100.0f * 0.4f, 100.0f * 0.9f, 100.0f * 1.0f));
	prog.setUniform("Light[0].Position", view * lightPos);
	prog.setUniform("EdgeWidth", 0.015f);
	prog.setUniform("PctExtend", 0.25f);
	prog.setUniform("LineColour", glm::vec4(0.05f, 0.0f, 0.05f, 1.0f));

	prog.setUniform("Light[1].L", glm::vec3(0.3f));
	prog.setUniform("Light[1].Position", glm::vec4(0, 0.15f, -1.0f, 0));
	prog.setUniform("Light[2].L", glm::vec3(5.0f));
	prog.setUniform("Light[2].Position", view * glm::vec4(-7, 3, 7, 1));
}

void Scene_Pbr::update(float t) {
	float deltaT = t - tPrev;
	if (tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;
	if (animating()) {
		lightAngle = glm::mod(lightAngle + deltaT * lightRotationSpeed, glm::two_pi<float>());
		lightPos.x = glm::cos(lightAngle) * 7.0f;
		lightPos.y = 3.0f;
		lightPos.z = glm::sin(lightAngle) * 7.0f;
	}
}

void Scene_Pbr::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	prog.setUniform("Light[0].Position", view * lightPos);
	prog.setUniform("MaterialColour", glm::vec4(0.7255f, 0.255f, 0.055f, 1.0f));

	drawScene();
}

void Scene_Pbr::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

void Scene_Pbr::setMatrices()
{
	glm::mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix", glm::mat3(mv));
	prog.setUniform("MVP", projection * mv);
}

void Scene_Pbr::compile() {
	try {
		prog.compileShader("shader/pbr.vert");
		prog.compileShader("shader/pbr.frag");
		//prog.compileShader("shader/pbr.geom");
		prog.link();
		prog.use();
	}
	catch (GLSLProgramException& e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Scene_Pbr::drawScene() {
	drawFloor();

	float metalRough = 0.25f;
	drawSpot(glm::vec3(-0.2f, 0.0f, 0.0f), metalRough, 1, glm::vec3(0.542f, 0.497f, 0.449f));


}

void Scene_Pbr::drawFloor() {
	model = glm::mat4(1.0f);
	prog.setUniform("Material.Rough", 0.9f);
	prog.setUniform("Material.Metal", 0);
	prog.setUniform("Material.Colour", glm::vec3(0.1f));
	model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.0f));

	setMatrices();
	plane.render();
}

void Scene_Pbr::drawSpot(const glm::vec3& pos, float rough, int metal, const glm::vec3& color) {
	model = glm::mat4(1.0f);
	prog.setUniform("Material.Rough", rough);
	prog.setUniform("Material.Metal", metal);
	prog.setUniform("Material.Colour", color);
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f));

	setMatrices();
	mesh->render();
}