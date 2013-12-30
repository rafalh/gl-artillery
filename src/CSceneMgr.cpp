#include "CSceneMgr.h"
#include "CRenderer.h"

CSceneMgr::CSceneMgr(CRenderer *pRenderer):
    m_pSkybox(nullptr), m_pCamera(nullptr),
    m_pRenderer(pRenderer), m_SkyboxProgram(0) {}

CSceneMgr::~CSceneMgr()
{
    for(CSceneNode *pNode: m_Nodes)
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
    
    if(m_pSkybox)
    {
        if(!m_SkyboxProgram)
            m_SkyboxProgram = m_pRenderer->loadShaders("shaders/SkyboxVS.glsl", "shaders/SkyboxFS.glsl");
        GLuint OldProgram = m_pRenderer->getProgram();
        m_pRenderer->setProgram(m_SkyboxProgram);
        glDepthMask(GL_FALSE);
        m_pSkybox->render();
        glDepthMask(GL_TRUE);
        m_pRenderer->setProgram(OldProgram);
    }
    
    for(CSceneNode *pNode: m_Nodes)
        pNode->render();
}
