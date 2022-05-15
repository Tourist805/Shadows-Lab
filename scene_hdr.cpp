#include "scene_hdr.h"

#include <iostream>
#include <sstream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

Scene_Hdr::Scene_Hdr() : angle(0.0f), tPrev(0.0f),
                         bloomBufWidth(0), bloomBufHeight(0),
                         plane(20.0f, 10.0f, 1, 1),
                        teapot(14, glm::mat4(1.0)), sphere(2.0f, 50, 50), cube(4.0f), torus(5.0f, 2.0f, 20, 2)
{

}

void Scene_Hdr::initScene() 
{
    compile();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    vec3 intense = vec3(0.6f);
    prog.setUniform("Lights[0].L", intense);
    prog.setUniform("Lights[1].L", intense);
    prog.setUniform("Lights[2].L", intense);

    intense = vec3(0.2f);
    prog.setUniform("Lights[0].La", intense);
    prog.setUniform("Lights[1].La", intense);
    prog.setUniform("Lights[2].La", intense);

    projection = mat4(1.0f);

    angle = glm::pi<float>() / 2.0f;

    setupFBO();

    // Array for full-screen quad
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
    glGenVertexArrays(1, &fsQuad);
    glBindVertexArray(fsQuad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    prog.setUniform("LumThresh", 1.7f);

    float weights[10], sum, sigma2 = 25.0f;

    // Compute and sum the weights
    weights[0] = gauss(0, sigma2);
    sum = weights[0];
    for (int i = 1; i < 10; i++) {
        weights[i] = gauss(float(i), sigma2);
        sum += 2 * weights[i];
    }

    // Normalize the weights and set the uniform
    for (int i = 0; i < 10; i++) {
        std::stringstream uniName;
        uniName << "Weight[" << i << "]";
        float val = weights[i] / sum;
        prog.setUniform(uniName.str().c_str(), val);
    }

    // Set up two sampler objects for linear and nearest filtering
    GLuint samplers[2];
    glGenSamplers(2, samplers);
    linearSampler = samplers[0];
    nearestSampler = samplers[1];

    GLfloat border[] = { 0.0f,0.0f,0.0f,0.0f };
    // Set up the nearest sampler
    glSamplerParameteri(nearestSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(nearestSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(nearestSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(nearestSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(nearestSampler, GL_TEXTURE_BORDER_COLOR, border);

    // Set up the linear sampler
    glSamplerParameteri(linearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(linearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(linearSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(linearSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(linearSampler, GL_TEXTURE_BORDER_COLOR, border);

    // We want nearest sampling except for the last pass.
    glBindSampler(0, nearestSampler);
    glBindSampler(1, nearestSampler);
    glBindSampler(2, nearestSampler);

    prog.setUniform("HdrTex", 0);
    prog.setUniform("BlurTex1", 1);
    prog.setUniform("BlurTex2", 2);

}

void Scene_Hdr::setupFBO() {
    // Generate and bind the framebuffer
    glGenFramebuffers(1, &hdrFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo);

    // Create the texture object
    glGenTextures(1, &hdrTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, width, height);

    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTex, 0);

    // Create the depth buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    // Bind the depth buffer to the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, depthBuf);

    // Set the targets for the fragment output variables
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    // Create an FBO for the bright-pass filter and blur
    glGenFramebuffers(1, &blurFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);

    // Create two texture objects to ping-pong for the bright-pass filter
    // and the two-pass blur
    bloomBufWidth = width / 8;
    bloomBufHeight = height / 8;
    glGenTextures(1, &tex1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, bloomBufWidth, bloomBufHeight);

    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, &tex2);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, bloomBufWidth, bloomBufHeight);

    // Bind tex1 to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

    glDrawBuffers(1, drawBuffers);

    // Unbind the framebuffer, and revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene_Hdr::update(float t)
{
    view = Camera::getCamera().getView();
    //mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    //mat4 view = camera.GetViewMatrix();

    //float deltaT = t - tPrev;
    //if (tPrev == 0.0f)
    //    deltaT = 0.0f;

    //tPrev = t;

    //angle += 30.0f * deltaT;
    //if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void Scene_Hdr::processInput()
{
    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //{
    //    std::cout << "W pressed" << endl;
    //    camera.ProcessKeyboard(FORWARD, deltaTime);
    //}
    //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //    camera.ProcessKeyboard(BACKWARD, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //    camera.ProcessKeyboard(LEFT, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //    camera.ProcessKeyboard(RIGHT, deltaTime);

}

void Scene_Hdr::render()
{
    pass1();
    computeLogAveLuminance();
    pass2();
    pass3();
    pass4();
    pass5();
}

void Scene_Hdr::computeLogAveLuminance()
{
    int size = width * height;
    std::vector<GLfloat> texData(size * 3);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTex);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, texData.data());
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        float lum = glm::dot(vec3(texData[i * 3 + 0], texData[i * 3 + 1], texData[i * 3 + 2]),
            vec3(0.2126f, 0.7152f, 0.0722f));
        sum += logf(lum + 0.00001f);
    }
    prog.setUniform("AveLum", expf(sum / size));
}


void Scene_Hdr::pass1()
{
    prog.setUniform("Pass", 1);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glViewport(0, 0, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, hdrFbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    view = glm::lookAt(vec3(2.0f, 0.0f, 14.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.3f, 100.0f);

    drawScene();
}

void Scene_Hdr::pass2()
{
    prog.setUniform("Pass", 2);

    glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);

    // We're writing to tex1 this time
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

    glViewport(0, 0, bloomBufWidth, bloomBufHeight);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    model = mat4(1.0f);
    view = mat4(1.0f);
    projection = mat4(1.0f);
    setMatrices();

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Scene_Hdr::pass3()
{
    prog.setUniform("Pass", 3);
    // We're writing to tex2 this time
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2, 0);

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Scene_Hdr::pass4()
{
    prog.setUniform("Pass", 4);
    // We're writing to tex1 this time
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Scene_Hdr::pass5()
{
    prog.setUniform("Pass", 5);

    // Bind to the default framebuffer, this time we're going to
    // actually draw to the screen!
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width, height);

    // In this pass, we're reading from tex1 (unit 1) and we want
    // linear sampling to get an extra blur
    glBindSampler(1, linearSampler);

    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Revert to nearest sampling
    glBindSampler(1, nearestSampler);
}

void Scene_Hdr::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void Scene_Hdr::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

void Scene_Hdr::compile()
{
    try {
        prog.compileShader("shader/hdr.vert");
        prog.compileShader("shader/hdr.frag");
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

float Scene_Hdr::gauss(float x, float sigma2)
{
    double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
    double expon = -(x * x) / (2.0 * sigma2);
    return (float)(coeff * exp(expon));
}

void Scene_Hdr::drawScene()
{
    vec3 intense = vec3(1.0f);
    prog.setUniform("Lights[0].L", intense);
    prog.setUniform("Lights[1].L", intense);
    prog.setUniform("Lights[2].L", intense);

    vec4 lightPos = vec4(0.0, 0.0f, 1.0f, 0.0f);
    //lightPos.x = -7.0f;
    prog.setUniform("Lights[0].Position", view * lightPos);
    lightPos = vec4(0.0, 0.0f, 1.0f, 0.0f);
    prog.setUniform("Lights[1].Position", view * lightPos);
    lightPos = vec4(0.0f, 0.0f, 1.0f, 0.0f);
    prog.setUniform("Lights[2].Position", view * lightPos);

    prog.setUniform("Material.Kd", 0.7f, 0.5f, 0.6f);
    prog.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Shininess", 125.0f);

    // The backdrop plane
    model = glm::rotate(mat4(1.0f), glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    plane.render();

    // The bottom plane
    model = glm::translate(mat4(1.0f), vec3(0.0f, -5.0f, 0.0f));
    setMatrices();
    plane.render();

    // Cube
    prog.setUniform("Material.Kd", vec3(0.2f, 0.7f, 0.9f));
    model = glm::translate(mat4(1.0f), vec3(-3.0f, -3.0f, 2.0f));
    setMatrices();
    cube.render();

    // Top plane
    /*model = glm::translate(mat4(1.0f), vec3(5.0f, 5.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-100.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    plane.render();*/

    // Torus
    //prog.setUniform("Material.Kd", vec3(0.4f, 0.9f, 0.4f));
    //model = glm::translate(mat4(1.0f), vec3(-3.0f, -3.0f, 2.0f));
    //setMatrices();
    //torus.render();

    // Sphere
    prog.setUniform("Material.Kd", vec3(0.4f, 0.9f, 0.4f));
    model = glm::translate(mat4(1.0f), vec3(-3.0f, -3.0f, 2.0f));
    setMatrices();
    sphere.render();

    // Teapot
    //prog.setUniform("Material.Kd", vec3(0.4f, 0.4f, 0.9f));
    //model = glm::translate(mat4(1.0f), vec3(3.0f, -5.0f, 6.0f));
    //model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    //setMatrices();
    //teapot.render();
}