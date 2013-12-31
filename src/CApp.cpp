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
#include "CShadersMgr.h"
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
    m_pSceneMgr->add(new CHeightMap("textures/hmap5.png", m_pRenderer));
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
    
    // Set main shader program
    m_pRenderer->setProgram(CShadersMgr::getInstance().get("main"));
    
    while(!m_pRenderer->isClosed())
    {
        m_pRenderer->clear();
        
        m_pSceneMgr->render();
        
        // Now render new frame in window and handle input events
        m_pRenderer->run();
        
        // new frame has been rendered
        m_pFpsCounter->onFrame();
    }
}

void CApp::onFpsChange()
{
    // Update window title
    char szBuf[256];
    sprintf(szBuf, "Artillery Animation - FPS: %.1f (max. %.1f)", m_pFpsCounter->getFps(), m_pFpsCounter->getMaxFps());
    m_pRenderer->setTitle(szBuf);
}
