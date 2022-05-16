#include "scene_rustMetal.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


Scene_RustMetal::Scene_RustMetal() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f),
					teapot(14, mat4(1.0f)), sky(100.0f)
{

}

void Scene_RustMetal::initScene()
{
	compile();

	glEnable(GL_DEPTH_TEST);

	projection = mat4(1.0f);

	angle = glm::radians(90.0f);

	GLuint cubeTex = Texture::loadHdrCubeMap("../Shadows-Lab/media/texture/cube/pisa-hdr/pisa");

	GLuint noiseTex = NoiseTex::generate2DTex(16.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, noiseTex);

	prog.use();
	prog.setUniform("CubeMapTex", 0);
	prog.setUniform("NoiseTex", 1);

	skyProg.use();
	skyProg.setUniform("SkyboxTex", 0);
}

void Scene_RustMetal::update(float t)
{
	float deltaT = t - tPrev;

	if (tPrev == 0.0f)
		deltaT = 0.0f;

	tPrev = t;

	angle += rotSpeed * deltaT;

	if (angle > glm::two_pi<float>())
		angle -= glm::two_pi<float>();
}

void Scene_RustMetal::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
	view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	// Skybox
	skyProg.use();
	model = mat4(1.0f);
	setMatrices(skyProg);
	sky.render();

	// Scene
	prog.use();
	prog.setUniform("WorldCameraPosition", cameraPos);
	prog.setUniform("MaterialColour", vec4(0.7255f, 0.255f, 0.055f, 1.0f));
	prog.setUniform("ReflectFactor", 0.85f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, -1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	setMatrices(prog);
	teapot.render();
}

void Scene_RustMetal::setMatrices(GLSLProgram& p)
{
	mat4 mv = view * model;

	p.setUniform("ModelMatrix", model);
	p.setUniform("MVP", projection * mv);
}

void Scene_RustMetal::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(50.0f), (float)w / h, 0.3f, 100.0f);
}

void Scene_RustMetal::compile()
{
	try {
		prog.compileShader("shader/rust_metal.vert");
		prog.compileShader("shader/rust_metal.frag");
		prog.link();

		skyProg.compileShader("shader/skybox.vert");
		skyProg.compileShader("shader/skybox.frag");
		skyProg.link();

		prog.use();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}