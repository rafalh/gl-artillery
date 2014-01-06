#include "stdafx.h"
#include "CGlowmap.h"
#include "CFrameBuffer.h"
#include "CMesh.h"
#include "CGeometryBuilder.h"
#include "CTextureMgr.h"
#include "CShadersMgr.h"
#include "CRenderer.h"

CGlowmap::CGlowmap(CRenderer *pRenderer, int Size):
    m_pRenderer(pRenderer), m_Size(Size)
{
    CGeometryBuilder Builder;
    Builder.addQuad(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 1.0f, 0.0f));
    m_pQuadMesh = Builder.createMesh();
    
    m_pGlowmap = new CFrameBuffer(Size, Size);
    m_pGlowmap2 = new CFrameBuffer(Size, Size, false);
    m_pGlowmap3 = new CFrameBuffer(Size, Size, false);
}

CGlowmap::~CGlowmap()
{
    delete m_pGlowmap;
    delete m_pGlowmap2;
    delete m_pGlowmap3;
    delete m_pQuadMesh;
}

void CGlowmap::begin()
{
    m_pGlowmap->bind();
}

void CGlowmap::end()
{
    m_pGlowmap->unbind();
    
    GLuint OldProgram = m_pRenderer->setProgram(CShadersMgr::getInstance().get("blur"));
    m_pRenderer->setProgramUniform("TexelSize", glm::vec2(1.0f/m_Size, 1.0f/m_Size));
    m_pRenderer->setProgramUniform("IsVertical", 0);
    
    m_pGlowmap2->bind();
    m_pRenderer->clear();
    m_pRenderer->setTexture(m_pGlowmap->getColor());
    m_pQuadMesh->render();
    m_pGlowmap2->unbind();
    
    m_pRenderer->setProgramUniform("IsVertical", 1);
    m_pGlowmap3->bind();
    m_pRenderer->clear();
    m_pRenderer->setTexture(m_pGlowmap2->getColor());
    m_pQuadMesh->render();
    m_pGlowmap3->unbind();
    
    m_pRenderer->setProgram(OldProgram);
}

void CGlowmap::render()
{
    GLuint OldProgram = m_pRenderer->setProgram(CShadersMgr::getInstance().get("texture"));
    m_pRenderer->setTexture(m_pGlowmap3->getColor());
    //m_pRenderer->setTexture(CTextureMgr::getInstance().get("textures/nightsky_west.png"));
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    m_pQuadMesh->render();
    glDisable(GL_BLEND);
    
    m_pRenderer->setProgram(OldProgram);
}
