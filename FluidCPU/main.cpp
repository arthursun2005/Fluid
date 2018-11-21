//
//  main.cpp
//  Fluid
//
//  Created by Arthur Sun on 10/22/18.
//  Copyright © 2018 Arthur Sun. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <thread>
#include "Fluid.hpp"

Fluid fluid;

GLuint WIDTH = 1000;
GLuint HEIGHT = 690;

GLFWwindow *window;

GLuint QuadVAO, QuadVBO, QuadProgram;

void InitFluid()
{
    QuadProgram = CreateProgram(&QuadVShader, &QuadFShader);
    glGenVertexArrays(1, &QuadVAO);
    glBindVertexArray(QuadVAO);
    glGenBuffers(1, &QuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Quad[0]) * sizeof(Quad), Quad, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    fluid.Init(WIDTH, HEIGHT);
}

void DeleteFluid()
{
    glDeleteVertexArrays(1, &QuadVAO);
    glDeleteBuffers(1, &QuadVBO);
    glDeleteProgram(QuadProgram);
}

void Run()
{
    fluid.Solve();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(QuadVAO);
    glUseProgram(QuadProgram);
    glBindTexture(GL_TEXTURE_2D, fluid.GetTexture());
    glUniform1i(glGetUniformLocation(QuadProgram, "buffer"), fluid.GetTexture());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

int main(int argc, const char * argv[]) {
    if(!glfwInit()){
        return 1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "FOGGY2D", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    InitFluid();
    do {
        glClear(GL_COLOR_BUFFER_BIT);
        Run();
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    } while (glfwWindowShouldClose(window) == 0);
    glfwTerminate();
    return 0;
}
