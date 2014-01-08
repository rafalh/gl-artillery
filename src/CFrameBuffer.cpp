#include "stdafx.h"
#include "CFrameBuffer.h"
#include "CLogger.h"
#include "utils.h"

GLint CFrameBuffer::m_OldViewport[4];

CFrameBuffer::CFrameBuffer(int w, int h, bool bDepthBuffer):
    m_Width(w), m_Height(h),
    m_DepthTexture(0)
{
    glGenFramebuffers(1, &m_FBO);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);CHECK_GL_ERROR();
    //glEnable(GL_TEXTURE_2D);
    CHECK_GL_ERROR();
    
    glGenTextures(1, &m_ColorTexture);
    glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);
    //GLenum Attachments[] = {GL_COLOR_ATTACHMENT0};
    //glDrawBuffers(1, Attachments);
    
    if(bDepthBuffer)
    {
        glGenTextures(1, &m_DepthTexture);
        glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
    }
    
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(Status != GL_FRAMEBUFFER_COMPLETE)
        CLogger::getInstance().warning("Framebuffer is not complete: %u\n", Status);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    CHECK_GL_ERROR();
}

CFrameBuffer::~CFrameBuffer()
{
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteTextures(1, &m_ColorTexture);
    if(m_DepthTexture)
        glDeleteTextures(1, &m_DepthTexture);
}

void CFrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    
    glGetIntegerv(GL_VIEWPORT, m_OldViewport);
    glViewport(0, 0, m_Width, m_Height);
    
    CHECK_GL_ERROR();
}

void CFrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(m_OldViewport[0], m_OldViewport[1], m_OldViewport[2], m_OldViewport[3]);
    
    CHECK_GL_ERROR();
}
