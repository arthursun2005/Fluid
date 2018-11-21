//
//  Setup.hpp
//  Fluid
//
//  Created by Arthur Sun on 10/22/18.
//  Copyright © 2018 Arthur Sun. All rights reserved.
//

#ifndef Setup_hpp
#define Setup_hpp

#include <iostream>
#include <float.h>
#include <cmath>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

static GLuint TEXTURE_ID = 1;

void InitQuad();
void DeleteQuad();

void blit(GLuint fbo);

void GenPoly(float* vs, int n, float s, int offset);

GLuint CreateProgram(const char * _vs[], const char * _fs[]);

static const GLfloat Quad[] =
{
    -1.0f,  1.0f,
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f,  1.0f
};
/*
static const GLfloat Quad[] =
{
    0.0f, 2.0f,
    0.0f, 0.0f,
    2.0f, 0.0f,
    2.0f, 2.0f
};
*/
struct Shader
{
    GLuint program;
    
    Shader(){
    }
    
    void init(const char** vs, const char** fs){
        program = CreateProgram(vs, fs);
    }
    
    void bind(){
        glUseProgram(program);
    }
    
    void free(){
        glDeleteProgram(program);
    }
    
    void uniform1i(const char* n, int i0) const
    {
        glUniform1i(glGetUniformLocation(program, n), i0);
    }
    
    void uniform2i(const char* n, int i0, int i1) const
    {
        glUniform2i(glGetUniformLocation(program, n), i0, i1);
    }
    
    void uniform3i(const char* n, int i0, int i1, int i2) const
    {
        glUniform3i(glGetUniformLocation(program, n), i0, i1, i2);
    }
    
    void uniform4i(const char* n, int i0, int i1, int i2, int i3) const
    {
        glUniform4i(glGetUniformLocation(program, n), i0, i1, i2, i3);
    }
    
    void uniform1f(const char* n, float f0) const
    {
        glUniform1f(glGetUniformLocation(program, n), f0);
    }
    
    void uniform2f(const char* n, float f0, float f1) const
    {
        glUniform2f(glGetUniformLocation(program, n), f0, f1);
    }
    
    void uniform2f(const char* n, const glm::vec2& x) const
    {
        glUniform2f(glGetUniformLocation(program, n), x.x, x.y);
    }
    
    void uniform3f(const char* n, float f0, float f1, float f2) const
    {
        glUniform3f(glGetUniformLocation(program, n), f0, f1, f2);
    }
    
    void uniform3f(const char* n, const glm::vec3& x) const
    {
        glUniform3f(glGetUniformLocation(program, n), x.x, x.y, x.z);
    }
    
    void uniform4f(const char* n, float f0, float f1, float f2, float f3) const
    {
        glUniform4f(glGetUniformLocation(program, n), f0, f1, f2, f3);
    }
};

extern GLuint QuadVAO;
extern GLuint QuadVBO;
extern Shader QuadShader;
extern const char *QuadVShader;
extern const char *QuadFShader;

struct Texture
{
    GLuint texture;
    GLuint id;
    Texture(){
        id = TEXTURE_ID++;
    }
    void init(GLenum format, GLuint x, GLuint y)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    void free()
    {
        glDeleteTextures(1, &texture);
    }
    void bind() const
    {
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    static void bind0()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

struct FrameBuffer
{
    GLuint fbo;
    FrameBuffer(){}
    void init(){
        glGenFramebuffers(1, &fbo);
    }
    void free(){
        glDeleteFramebuffers(1, &fbo);
    }
    void bind(const Texture& texture) const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.texture, 0);
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    static void bind(GLuint x)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, x, 0);
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }
};

inline void checkShader(GLuint x){
    int status, length;
    glGetProgramiv(x, GL_LINK_STATUS, &status);
    glGetProgramiv(x, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        GLchar log[length];
        glGetProgramInfoLog(x, length, nullptr, log);
        printf("%s \n", log);
    }
}

#endif /* Setup_hpp */
