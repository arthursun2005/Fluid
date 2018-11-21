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
        velocityBuffer.Free();
        glDeleteProgram(velocityProgram);
        glDeleteProgram(colorProgram);
    }
    
    void Init(GLuint x, GLuint y)
    {
        velocityBuffer.Init(GL_RG, x, y);
        colorBuffer.Init(GL_RGBA, x, y);
        velocityProgram = CreateProgram(&baseVShader, &velocityFShader);
        colorProgram = CreateProgram(&baseVShader, &colorFShader);
    }
    
    void Solve()
    {
        glUseProgram(velocityProgram);
        blit(velocityProgram, velocityBuffer);
        velocityBuffer.swap();
        
        glUseProgram(colorProgram);
        glUniform1i(glGetUniformLocation(colorProgram, "colors"), colorBuffer.texIds[1]);
        blit(colorProgram, colorBuffer);
        colorBuffer.swap();
    }
    
    void blit(GLuint program, const DoubleBufferFrameBuffer& buffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer.fbos[0]);
        glBindVertexArray(QuadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    inline GLuint GetTexture()
    {
        return colorBuffer.texIds[1];
    }
    
private:
    
    static const char *baseVShader;
    
    static const char *velocityFShader;
    
    static const char *colorFShader;
    
    static GLuint velocityProgram;
    static GLuint colorProgram;
    
    DoubleBufferFrameBuffer velocityBuffer;
    DoubleBufferFrameBuffer colorBuffer;
};

#endif /* Fluid_hpp */
