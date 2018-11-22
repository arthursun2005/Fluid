//
//  Fluid.hpp
//  Fluid
//
//  Created by Arthur Sun on 10/22/18.
//  Copyright © 2018 Arthur Sun. All rights reserved.
//

#ifndef Fluid_hpp
#define Fluid_hpp

#include "Setup.hpp"

struct Fluid
{
    Fluid(){}
    
    ~Fluid(){
        velocity[0].free();
        velocity[1].free();
        color[0].free();
        color[1].free();
        glDeleteVertexArrays(1, &forceVAO);
        velocityShader.free();
        colorShader.free();
        forceShader.free();
        fbo.free();
    }
    
    void init(GLuint x, GLuint y)
    {
        glGenVertexArrays(1, &forceVAO);
        fbo.init();
        velocity[0].init(GL_RG, x, y);
        velocity[1].init(GL_RG, x, y);
        color[0].init(GL_RGBA, x, y);
        color[1].init(GL_RGBA, x, y);
        
        velocityShader.init(&baseVShader, &velocityFShader);
        colorShader.init(&baseVShader, &colorFShader);
        forceShader.init(&baseVShader, &forceFShader);
        
        sizeX = x;
        sizeY = y;
    }
    
    void ApplyForce(GLfloat* vertices, GLuint count, glm::vec2 _velocity, const glm::vec2& p, const glm::vec2& r)
    {
        GLuint vbo;
        velocity[0].bind();
        size_t size = 2 * count * sizeof(GLfloat);
        
        forceShader.bind();

        forceShader.uniform1i("velocities", velocity[1].id);
        forceShader.uniform2f("size", sizeX, sizeY);
        forceShader.uniform2f("t_r", r.x, r.y);
        forceShader.uniform2f("t_p", p.x, p.y);
        forceShader.uniform2f("t_s", 1.0f, 1.0f);
        
        forceShader.uniform2f("force", _velocity.x, _velocity.y);

        glBindVertexArray(forceVAO);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        
        fbo.bind(velocity[0]);
        
        glBindFramebuffer(GL_FRAMEBUFFER, fbo.fbo);
        
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_FAN, 0, count);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);
        
        std::swap(velocity[0], velocity[1]);
    }
    
    void solve()
    {
        velocity[0].bind();
        velocityShader.bind();
        velocityShader.uniform1i("velocities", velocity[1].id);
        velocityShader.uniform2f("size", sizeX, sizeY);
        velocityShader.uniform2f("t_r", 1.0f, 0.0f);
        velocityShader.uniform2f("t_p", 0.0f, 0.0f);
        velocityShader.uniform2f("t_s", 1.0f, 1.0f);
        fbo.bind(velocity[0]);
        blit(fbo.fbo);
        std::swap(velocity[0], velocity[1]);
        
        color[0].bind();
        colorShader.bind();
        colorShader.uniform1i("velocities", velocity[1].id);
        colorShader.uniform2f("size", sizeX, sizeY);
        colorShader.uniform2f("t_r", 1.0f, 0.0f);
        colorShader.uniform2f("t_p", 0.0f, 0.0f);
        colorShader.uniform2f("t_s", 1.0f, 1.0f);
        fbo.bind(color[0]);
        blit(fbo.fbo);
        std::swap(color[0], color[1]);
    }
    
    inline GLuint texture()
    {
        return color[1].id;
    }
    
private:
    
    static const char *baseVShader;
    
    static const char *velocityFShader;
    static const char *colorFShader;
    static const char *forceFShader;
    
    static Shader velocityShader;
    static Shader colorShader;
    static Shader forceShader;
    
    static GLuint forceVAO;
    
    GLuint sizeX, sizeY;
    
    Texture velocity[2];
    Texture color[2];
    FrameBuffer fbo;
};

#endif /* Fluid_hpp */
