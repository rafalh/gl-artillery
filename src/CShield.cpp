#include "stdafx.h"
#include "CShield.h"
#include "CRenderer.h"
#include "CGeometryBuilder.h"
#include "CTextureMgr.h"
#include "CShadersMgr.h"
#include "CMesh.h"

CShield::CShield(CRenderer *pRenderer):
    m_pRenderer(pRenderer), m_pMesh(nullptr)
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
}

void CShield::render()
{
    GLuint OldProgram = m_pRenderer->setProgram(CShadersMgr::getInstance().get("shield"));
    
    m_pRenderer->setModelTransform(m_Transform);
    m_pRenderer->setDoubleSided(true);
    //m_pRenderer->setTexture(CTextureMgr::getInstance().get("textures/test.png"));
    m_pRenderer->setProgramUniform("Time", glfwGetTime());
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    m_pMesh->render();
    
    glDisable(GL_BLEND);
    
    m_pRenderer->setDoubleSided(false);
    m_pRenderer->setProgram(OldProgram);
}
