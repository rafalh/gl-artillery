#include "stdafx.h"
#include "CApp.h"
#include "CRenderer.h"
#include "CLogger.h"
#include "CSceneMgr.h"
#include "CShadersMgr.h"
#include "CHeightMap.h"
#include "CCity.h"
#include "CSkybox.h"
#include "CShield.h"
#include "CCannon.h"
#include "CCamera.h"
#include "CInputHandler.h"
#include "utils.h"

using namespace glm;

CApp::CApp()
{
    m_pRenderer = new CRenderer(800, 600);
    m_pSceneMgr = new CSceneMgr(m_pRenderer);
    
    CCamera *pCam = new CCamera();
    pCam->setPosition(vec3(60.0f, 0.0f, 60.0f));
    
    CInputHandler *pInputHandler = new CInputHandler(m_pRenderer);
    pInputHandler->setCamera(pCam);
    m_pRenderer->setInputHandler(pInputHandler);
    m_pSceneMgr->setCamera(pCam);
    
    m_pSceneMgr->setSkybox(new CSkybox("textures/nightsky_%s.png", m_pRenderer));
    m_pSceneMgr->add(new CHeightMap("textures/hmap.tif", m_pRenderer));
    m_pSceneMgr->add(new CCity(vec3(-45.0f, 11.0f, 45.0f), m_pRenderer));
    m_pSceneMgr->add(new CShield(vec3(-45.0f, 7.0f, 45.0f), m_pRenderer));
    m_pSceneMgr->add(new CCannon(vec3(34.0f, 33.9f, -34.0f), m_pRenderer));
    
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
