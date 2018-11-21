//
//  Setup.cpp
//  Fluid
//
//  Created by Arthur Sun on 10/23/18.
//  Copyright © 2018 Arthur Sun. All rights reserved.
//

#include "Setup.hpp"

// 0 = screen texture
GLuint DoubleBufferFrameBuffer::Id = 1;

GLuint CreateProgram(const char * _vs[], const char * _fs[]){
    GLuint programID = glCreateProgram();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, _vs, nullptr);
    glShaderSource(fs, 1, _fs, nullptr);
    glCompileShader(vs);
    glCompileShader(fs);
    glAttachShader(programID, vs);
    glAttachShader(programID, fs);
    glLinkProgram(programID);
    glDetachShader(programID, vs);
    glDetachShader(programID, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return programID;
}

const char *QuadVShader = (char*)R"glsl(

#version 330 core

layout (location = 0) in vec2 position;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    //TexCoord = position;
    TexCoord = vec2(position.x < 0 ? 0 : position.x, position.y < 0 ? 0 : position.y);
}

)glsl";

const char *QuadFShader = (char*)R"glsl(

#version 330 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D buffer;

void main()
{
    color = texture(buffer, TexCoord);
}

)glsl";
