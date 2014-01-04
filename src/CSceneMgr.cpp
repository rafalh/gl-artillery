#include "CSceneMgr.h"
#include "CRenderer.h"
#include "CShadersMgr.h"

CSceneMgr::CSceneMgr(CRenderer *pRenderer):
    m_pSkybox(nullptr), m_pCamera(nullptr),
    m_pRenderer(pRenderer), m_SkyboxProgram(0) {}

CSceneMgr::~CSceneMgr()
{
    for(CSceneNode *pNode: m_Nodes)
        delete pNode;
    
    for(CSceneNode *pNode: m_TransparentNodes)
        delete pNode;
    
    delete m_pSkybox;
    delete m_pCamera;
    
    if(m_SkyboxProgram)
        glDeleteProgram(m_SkyboxProgram);
}

void CSceneMgr::render()
{
    m_pRenderer->setProjMatrix(m_pCamera->getProjMatrix());
    m_pRenderer->setViewMatrix(m_pCamera->getViewMatrix());
    m_pRenderer->setProgramUniform("EyePos", m_pCamera->getPosition());
    
    for(CSceneNode *pNode: m_Nodes)
        pNode->animate();
    for(CSceneNode *pNode: m_TransparentNodes)
        pNode->animate();
    
    glDepthMask(GL_FALSE);
    if(m_pSkybox)
        m_pSkybox->render();
    glDepthMask(GL_TRUE);
    
    for(CSceneNode *pNode: m_Nodes)
        pNode->render();
    
    glDepthMask(GL_FALSE);
    for(CSceneNode *pNode: m_TransparentNodes)
        pNode->render();
    glDepthMask(GL_TRUE);
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
