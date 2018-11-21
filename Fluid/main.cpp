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

int WIDTH = 800;
int HEIGHT = 800;

GLFWwindow *window;

GLuint QuadVAO, QuadVBO;
Shader QuadShader;

double mouseX = 0.0f;
double mouseY = 0.0f;
double pmouseX = 0.0f;
double pmouseY = 0.0f;

const int ns = 5;

float pvs[2 * ns];

Shader s;

glm::vec2 arg(float a)
{
    return glm::vec2(cosf(a), sinf(a));
}

void Run()
{
    fluid.solve();
    Texture::bind0();
    QuadShader.bind();
    QuadShader.uniform1i("T", fluid.texture());
    QuadShader.uniform2f("size", WIDTH, HEIGHT);
    QuadShader.uniform2f("t_r", arg(0.0f));
    QuadShader.uniform2f("t_p", 0.0f, 0.0f);
    FrameBuffer::bind(0);
    blit(0);
}

void MouseDragged()
{
}

void MousePressed(GLFWwindow* window, int button, int action, int mods)
{
    fluid.ApplyForce(pvs, ns, glm::vec2(0.0f, 0.03f), glm::vec2(mouseX, mouseY), arg(0.0f));
}

void Norm(double *x, double *y, double a, double b)
{
    *x = a * (*x * 2.0f/WIDTH - 1.0f);
    *y = b * (*y * 2.0f/HEIGHT - 1.0f);
}

void OnResize(GLFWwindow* window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
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
    glewInit();
    InitQuad();
    fluid.init(WIDTH, HEIGHT);
    glfwSetWindowSizeCallback(window, OnResize);
    glfwSetMouseButtonCallback(window, MousePressed);
    GenPoly(pvs, ns, 0.4f, 0);
    do {        
        glfwGetCursorPos(window, &mouseX, &mouseY);
        Norm(&mouseX, &mouseY, 1.0f, -1.0f);
        
        Run();
        
        pmouseX = mouseX;
        pmouseY = mouseY;
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    } while (glfwWindowShouldClose(window) == GL_FALSE);
    DeleteQuad();
    glfwTerminate();
    return 0;
}
