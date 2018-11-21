//
//  Setup.cpp
//  Fluid
//
//  Created by Arthur Sun on 10/23/18.
//  Copyright © 2018 Arthur Sun. All rights reserved.
//

#include "Setup.hpp"

void InitQuad()
{
    QuadShader.init(&QuadVShader, &QuadFShader);
    glGenVertexArrays(1, &QuadVAO);
    glBindVertexArray(QuadVAO);
    
    glGenBuffers(1, &QuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DeleteQuad()
{
    glDeleteVertexArrays(1, &QuadVAO);
    glDeleteBuffers(1, &QuadVBO);
    QuadShader.free();
}

void blit(GLuint fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindVertexArray(QuadVAO);
    //glViewport(0, 0, 800, 800);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint CreateProgram(const char * _vs[], const char * _fs[]){
    GLuint programID = glCreateProgram();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, _vs, nullptr);
    glShaderSource(fs, 1, _fs, nullptr);
    glCompileShader(vs);
    glCompileShader(fs);
    checkShader(vs);
    checkShader(fs);
    glAttachShader(programID, vs);
    glAttachShader(programID, fs);
    glLinkProgram(programID);
    glDetachShader(programID, vs);
    glDetachShader(programID, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return programID;
}

void GenPoly(float* vs, int n, float s, int offset)
{
    float a = 2.0f * M_PI / n;
    float k_c = cosf(a);
    float k_s = sinf(a);
    float x = s;
    float y = 0.0f;
    for(int i = 0; i < n; ++i)
    {
        vs[i * 2 + offset] = x;
        vs[i * 2 + 1 + offset] = y;
        x = x * k_c - y * k_s;
        y = vs[i * 2 + offset] * k_s + y * k_c;
    }
}

const char *QuadVShader = (char*)R"(

#version 410 core

layout (location = 0) in vec2 position;
uniform vec2 t_p;
uniform vec2 t_r;

void main()
{
    vec2 pos = vec2(t_p.x + position.x * t_r.x - position.y * t_r.y, t_p.y + position.x * t_r.y + position.y * t_r.x);
    gl_Position = vec4(pos, 0.0, 1.0);
}

)";

const char *QuadFShader = (char*)R"(

#version 410 core

out vec4 color;

uniform vec2 size;
uniform sampler2D T;

void main()
{
    vec2 coord = gl_FragCoord.xy / size;
    color = texture(T, coord);
}

)";
