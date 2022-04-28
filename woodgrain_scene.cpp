#include "woodgrain_scene.h"
#include "helper/noisetex.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

WoodGrain_Scene::WoodGrain_Scene() { }

void WoodGrain_Scene::initScene()
{
    compile();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);
    // Array for quad
    GLfloat verts[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    GLfloat tc[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    // Set up the buffers
    unsigned int handle[2];
    glGenBuffers(2, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    // Set up the vertex array object
    glGenVertexArrays(1, &quad);
    glBindVertexArray(quad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(1);  // Texture coordinates

    glBindVertexArray(0);

    prog.setUniform("Colour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("NoiseTex", 0);

    mat4 slice;
    slice = glm::rotate(slice, glm::radians(10.0f), vec3(1.0, 0.0, 0.0));
    slice = glm::rotate(slice, glm::radians(-20.0f), vec3(0.0, 0.0, 1.0));
    slice = glm::scale(slice, vec3(40.0, 40.0, 1.0));
    slice = glm::translate(slice, vec3(-0.35, -0.5, 2.0));

    prog.setUniform("Slice", slice);

    GLuint noiseTex = NoiseTex::generate2DTex(6.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noiseTex);
}

void WoodGrain_Scene::update(float t) { }

void WoodGrain_Scene::render()
{
    view = mat4(1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glFinish();
}

void WoodGrain_Scene::drawScene()
{
    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(quad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void WoodGrain_Scene::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("MVP", projection * mv);
}

void WoodGrain_Scene::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}

void WoodGrain_Scene::compile()
{
    try {
        prog.compileShader("shader/wood_grain.vert");
        prog.compileShader("shader/wood_grain.frag");
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}