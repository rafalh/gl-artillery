#include "CSceneMgr.h"
#include "CRenderer.h"
#include "CShadersMgr.h"
#include "CGlowmap.h"

CSceneMgr::CSceneMgr(CRenderer *pRenderer):
    m_pSkybox(nullptr), m_pCamera(nullptr),
    m_pRenderer(pRenderer), m_pGlowmap(nullptr),
    m_bHasEmissiveNodes(false) {}

CSceneMgr::~CSceneMgr()
{
    for(CSceneNode *pNode: m_Nodes)
        delete pNode;
    
    for(CSceneNode *pNode: m_TransparentNodes)
        delete pNode;
    
    delete m_pSkybox;
    delete m_pCamera;
    delete m_pGlowmap;
}

void CSceneMgr::add(CSceneNode *pNode)
{
    if(pNode->isTransparent())
        m_TransparentNodes.push_back(pNode);
    else
        m_Nodes.push_back(pNode);
    
    if(pNode->isEmissive())
        m_bHasEmissiveNodes = true;
}

void CSceneMgr::render()
{
    // Animate all nodes
    for(CSceneNode *pNode: m_Nodes)
        pNode->animate();
    for(CSceneNode *pNode: m_TransparentNodes)
        pNode->animate();
    
    // Prepare shader program
    m_pRenderer->setProjMatrix(m_pCamera->getProjMatrix());
    m_pRenderer->setViewMatrix(m_pCamera->getViewMatrix());
    m_pRenderer->setProgramUniform("EyePos", m_pCamera->getPosition());
    
    // Render skybox
    glDepthMask(GL_FALSE);
    if(m_pSkybox)
        m_pSkybox->render();
    glDepthMask(GL_TRUE);
    
    // Render opaque objects
    for(CSceneNode *pNode: m_Nodes)
        pNode->render();
    
    // Render transparent objects
    glDepthMask(GL_FALSE);
    for(CSceneNode *pNode: m_TransparentNodes)
        pNode->render();
    glDepthMask(GL_TRUE);
    
    // Add glow if there are emissive nodes
    if(m_bHasEmissiveNodes)
    {
        buildGlowmap();
        m_pGlowmap->render();
    }
}

void CSceneMgr::buildGlowmap()
{
    if(!m_pGlowmap)
        m_pGlowmap = new CGlowmap(m_pRenderer, 512);
    
    m_pGlowmap->begin();
    
    m_pRenderer->clear();
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    for(CSceneNode *pNode: m_Nodes)
    {
        if(!pNode->isEmissive())
            pNode->render();
    }
    for(CSceneNode *pNode: m_TransparentNodes)
    {
        if(!pNode->isEmissive())
            pNode->render();
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    for(CSceneNode *pNode: m_Nodes)
    {
        if(pNode->isEmissive())
            pNode->render();
    }
    for(CSceneNode *pNode: m_TransparentNodes)
    {
        if(pNode->isEmissive())
            pNode->render();
    }
    
    m_pGlowmap->end();
}

bool CSceneMgr::testCollision(const glm::vec3 RayBegin, const glm::vec3 RayEnd)
{
    bool bCol = false;
    
    for(CSceneNode *pNode: m_Nodes)
        bCol = bCol || pNode->testCollision(RayBegin, RayEnd);
    for(CSceneNode *pNode: m_TransparentNodes)
        bCol = bCol || pNode->testCollision(RayBegin, RayEnd);
    
    return bCol;
}
