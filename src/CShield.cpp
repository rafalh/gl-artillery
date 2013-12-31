#include "stdafx.h"
#include "CShield.h"
#include "CRenderer.h"
#include "CGeometryBuilder.h"
#include "CTextureMgr.h"
#include "CMesh.h"

CShield::CShield(CRenderer *pRenderer):
    m_pRenderer(pRenderer), m_pMesh(nullptr), m_Program(0)
{
    m_pMesh = new CMesh;
    CGeometryBuilder Builder;
    Builder.setColor(0x8040A0FF);
    Builder.addIcosphere(2);
    m_pMesh->setVertices(Builder.getVertices().data(), Builder.getVertices().size());
    m_pMesh->setIndices(Builder.getIndices().data(), Builder.getIndices().size());
    m_Transform = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f)), glm::vec3(16.0f, 16.0f, 16.0f));
}

CShield::~CShield()
{
    delete m_pMesh;
    if(m_Program)
        glDeleteProgram(m_Program);
}

void CShield::render()
{
    if(!m_Program)
        m_Program = m_pRenderer->loadShaders("shaders/shield.vs.glsl", "shaders/shield.fs.glsl");
    GLuint OldProgram = m_pRenderer->setProgram(m_Program);
    
    m_pRenderer->setModelTransform(m_Transform);
    m_pRenderer->setDoubleSided(true);
    m_pRenderer->setTexture(CTextureMgr::getInstance().get("textures/test.png"));
    m_pRenderer->setProgramUniform("Time", glfwGetTime());
    
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    m_pMesh->render();
    
    glDisable(GL_BLEND);
    
    m_pRenderer->setDoubleSided(false);
    m_pRenderer->setProgram(OldProgram);
}
