#include "scene_paint.h"
#include "helper/noisetex.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

Scene_Paint::Scene_Paint() : teapot(14, mat4(1.0f)) { }

void Scene_Paint::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);

    prog.setUniform("NoiseTex", 0);

    GLuint noiseTex = NoiseTex::generate2DTex(30.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noiseTex);

    prog.setUniform("Light.L", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Light.La", vec3(0.2f, 0.2f, 0.7f));
    lightPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    prog.setUniform("PaintColour", vec3(0.2f, 0.3f, 0.2f));
    prog.setUniform("Threshold", 0.65f);
}

void Scene_Paint::update(float t) { }

void Scene_Paint::render()
{
    prog.setUniform("Light.Position", lightPos);

    vec3 cameraPos = 0.85f * vec3(0.0f, 5.0f, 6.0f);
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, -3.0f), vec3(0.0f, 1.0f, 0.0f));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glFinish();
}

void Scene_Paint::drawScene()
{
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-15.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();

    prog.setUniform("Material.Ka", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Kd", 0.7f, 0.5f, 0.3f);
    prog.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Shininess", 100.0f);

    teapot.render();
}

void Scene_Paint::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void Scene_Paint::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(55.0f), (float)width / height, 0.3f, 100.0f);
}

void Scene_Paint::compile()
{
    try {
        prog.compileShader("shader/paint_spatter.vert");
        prog.compileShader("shader/paint_spatter.frag");
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}