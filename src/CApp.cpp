#include "stdafx.h"
#include "CApp.h"
#include "CRenderer.h"
#include "CLogger.h"
#include "CSceneMgr.h"
#include "CHeightMap.h"
#include "CCity.h"
#include "CSkybox.h"
#include "CShield.h"
#include "CCamera.h"
#include "CInputHandler.h"
#include "utils.h"

CApp::CApp()
{
    m_pRenderer = new CRenderer(800, 600);
    m_pSceneMgr = new CSceneMgr(m_pRenderer);
    
    CCamera *pCam = new CCamera();
    CInputHandler *pInputHandler = new CInputHandler(m_pRenderer);
    pInputHandler->setCamera(pCam);
    m_pRenderer->setInputHandler(pInputHandler);
    m_pSceneMgr->setCamera(pCam);
    
    m_pSceneMgr->setSkybox(new CSkybox("textures/nightsky_%s.png", m_pRenderer));
    m_pSceneMgr->add(new CHeightMap(m_pRenderer));
    m_pSceneMgr->add(new CCity(m_pRenderer));
    m_pSceneMgr->add(new CShield(m_pRenderer));
    
    m_pFpsCounter = new CFpsCounter;
    m_pFpsCounter->addObserver(this);
}

CApp::~CApp()
{
    delete m_pSceneMgr;
    delete m_pFpsCounter;
    delete m_pRenderer;
}

void CApp::start()
{
    CLogger::getInstance().info("Starting animation!\n");
    
    // Compile OpenGL shaders
    glGetError();
	GLuint Program = m_pRenderer->loadShaders("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
	CHECK_GL_ERROR();
    
    // Set VS and FS
    m_pRenderer->setProgram(Program);
    
    while(!m_pRenderer->isClosed())
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        m_pSceneMgr->render();
        
        // Now render new frame in window and handle input events
        m_pRenderer->run();
        
        // new frame has been rendered
        m_pFpsCounter->onFrame();
    }
    
    if(Program)
        glDeleteProgram(Program);
}

void CApp::onFpsChange()
{
    // Update window title
    char szBuf[256];
    sprintf(szBuf, "Artillery Animation - FPS: %.1f (max. %.1f)", m_pFpsCounter->getFps(), m_pFpsCounter->getMaxFps());
    m_pRenderer->setTitle(szBuf);
}
