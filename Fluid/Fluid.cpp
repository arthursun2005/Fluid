//
//  Fluid.cpp
//  Fluid
//
//  Created by Arthur Sun on 10/22/18.
//  Copyright © 2018 Arthur Sun. All rights reserved.
//

#include "Fluid.hpp"

Shader Fluid::velocityShader;
Shader Fluid::colorShader;
Shader Fluid::forceShader;
GLuint Fluid::forceVAO;

const char *Fluid::baseVShader = QuadVShader;

const char *Fluid::velocityFShader = (char*)R"(
#version 410 core

out vec2 velocity;
uniform sampler2D velocities;
uniform vec2 size;

void main()
{
    vec2 coord = gl_FragCoord.xy / size;
    vec2 vel = texture(velocities, coord).xy;
    velocity = vel;
}
)";

const char *Fluid::colorFShader = (char*)R"(
#version 410 core

out vec4 color;
uniform vec2 size;
uniform sampler2D velocities;
uniform sampler2D colors;

void main()
{
    vec2 coord = gl_FragCoord.xy / size;
    vec2 vel = texture(velocities, coord).xy;
    float m = vel.x * vel.x + vel.y * vel.y;
    color = vec4(coord.x, m, coord.y, 1.0);
}
)";

const char *Fluid::forceFShader = (char*)R"(
#version 410 core

out vec2 velocity;
uniform vec2 size;
uniform sampler2D velocities;
uniform vec2 force;

void main()
{
    vec2 coord = gl_FragCoord.xy / size;
    velocity = texture(velocities, coord).xy + force;
}
)";
