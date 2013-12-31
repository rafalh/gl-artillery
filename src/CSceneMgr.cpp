#include "CSceneMgr.h"
#include "CRenderer.h"

CSceneMgr::CSceneMgr(CRenderer *pRenderer):
    m_pSkybox(nullptr), m_pCamera(nullptr),
    m_pRenderer(pRenderer), m_ProgramNoLight(0) {}

CSceneMgr::~CSceneMgr()
{
    for(CSceneNode *pNode: m_Nodes)
        delete pNode;
    
    for(CSceneNode *pNode: m_TransparentNodes)
        delete pNode;
    
    delete m_pSkybox;
    delete m_pCamera;
    
    if(m_ProgramNoLight)
        glDeleteProgram(m_ProgramNoLight);
}

void CSceneMgr::render()
{
    m_pRenderer->setProjMatrix(m_pCamera->getProjMatrix());
    m_pRenderer->setViewMatrix(m_pCamera->getViewMatrix());
    m_pRenderer->setProgramUniform("EyePos", m_pCamera->getPosition());
    
    if(!m_ProgramNoLight)
        m_ProgramNoLight = m_pRenderer->loadShaders("shaders/SkyboxVS.glsl", "shaders/SkyboxFS.glsl");
    
    GLuint OldProgram = m_pRenderer->setProgram(m_ProgramNoLight);
    glDepthMask(GL_FALSE);
    if(m_pSkybox)
        m_pSkybox->render();
    glDepthMask(GL_TRUE);
    m_pRenderer->setProgram(OldProgram);
    
    for(CSceneNode *pNode: m_Nodes)
        pNode->render();
    
    glDepthMask(GL_FALSE);
    for(CSceneNode *pNode: m_TransparentNodes)
        pNode->render();
    glDepthMask(GL_TRUE);
}
