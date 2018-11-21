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

#define min_buffer_capacity 32

static const GLfloat Quad[] =
{
    -1.0f,  1.0f,
    -1.0f, -1.0f,
    1.0f, -1.0f,
    
    -1.0f,  1.0f,
    1.0f, -1.0f,
    1.0f,  1.0f
};

extern GLuint QuadVAO;
extern GLuint QuadVBO;
extern GLuint QuadProgram;
extern const char *QuadVShader;
extern const char *QuadFShader;

struct DoubleBufferFrameBuffer
{
    GLuint fbos[2];
    GLuint textures[2];
    GLuint texIds[2];
    
    static GLuint Id;
    
    DoubleBufferFrameBuffer()
    {
        texIds[0] = Id;
        texIds[1] = Id + 1;
        Id += 2;
    }
    
    void Init(GLenum format, GLuint x, GLuint y)
    {
        glGenFramebuffers(2, fbos);
        glGenTextures(2, textures);
        
        for(int i=0;i<2;++i)
        {
            glActiveTexture(GL_TEXTURE0 + texIds[i]);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glBindFramebuffer(GL_FRAMEBUFFER, fbos[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[i], 0);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void Free()
    {
        glDeleteFramebuffers(2, fbos);
        glDeleteTextures(2, textures);
    }
    
    inline void swap()
    {
        std::swap(fbos[0], fbos[1]);
        std::swap(textures[0], textures[1]);
        std::swap(texIds[0], texIds[1]);
    }
};

GLuint CreateProgram(const char * _vs[], const char * _fs[]);

#endif /* Setup_hpp */
