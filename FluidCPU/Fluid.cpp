//
//  Fluid.cpp
//  Fluid
//
//  Created by Arthur Sun on 10/22/18.
//  Copyright © 2018 Arthur Sun. All rights reserved.
//

#include "Fluid.hpp"

GLuint Fluid::velocityProgram;
GLuint Fluid::colorProgram;

const char *Fluid::baseVShader = QuadVShader;

const char *Fluid::velocityFShader = (char*)R"glsl(
#version 330 core

in vec2 TexCoord;
out vec2 velocity;

void main()
{
    velocity = vec2(0.0, 0.0);
}
)glsl";

const char *Fluid::colorFShader = (char*)R"glsl(
#version 330 core

in vec2 TexCoord;
out vec4 color;
uniform sampler2D colors;

void main()
{
    vec4 src = texture(colors, TexCoord);
    color = src + vec4(0.01, 0.0, 0.001, 1.0);
}
)glsl";
