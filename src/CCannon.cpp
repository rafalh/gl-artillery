#include "stdafx.h"
#include "CCannon.h"
#include "CMesh.h"
#include "CRenderer.h"
#include "CGeometryBuilder.h"
#include "CTextureMgr.h"
#include "CLogger.h"

CCannon::CCannon(CRenderer *pRenderer):
    m_pRenderer(pRenderer), m_pMesh(nullptr)
{
    CGeometryBuilder Builder;
    
    Builder.setColor(0xFF33AACC);
    Builder.addTube(1.0f, 1.5f, 20);
    
    m_pMesh = new CMesh;
    m_pMesh->setVertices(Builder.getVertices().data(), Builder.getVertices().size());
    m_pMesh->setIndices(Builder.getIndices().data(), Builder.getIndices().size());
    
    m_Transform = glm::mat4(1.0f);
    m_Transform = glm::translate(m_Transform, glm::vec3(50.0f, 0.0f, 50.0f));
    m_Transform = glm::scale(m_Transform, glm::vec3(1.0f, 10.0f, 1.0f));
}

CCannon::~CCannon()
{
    delete m_pMesh;
}

void CCannon::render()
{
    m_pRenderer->setModelTransform(m_Transform);
    m_pRenderer->setTexture(CTextureMgr::getInstance().getWhite());
    m_pMesh->render();
}
