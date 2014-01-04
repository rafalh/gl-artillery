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

CApp::CApp():
    m_State(IDLE)
{
    m_StateChangeTime = glfwGetTime();
    
    m_pRenderer = new CRenderer(800, 600);
    m_pSceneMgr = new CSceneMgr(m_pRenderer);
    
    CCamera *pCam = new CCamera();
    pCam->setPosition(vec3(40.0f, 40.0f, -40.0f));
    
    CInputHandler *pInputHandler = new CInputHandler(m_pRenderer);
    pInputHandler->setCamera(pCam);
    m_pRenderer->setInputHandler(pInputHandler);
    m_pSceneMgr->setCamera(pCam);
    
    m_pSceneMgr->setSkybox(new CSkybox("textures/nightsky_%s.png", m_pRenderer));
    m_pSceneMgr->add(new CHeightMap("textures/hmap.tif", m_pRenderer));
    m_pSceneMgr->add(new CCity(vec3(-45.0f, 11.0f, 45.0f), m_pRenderer));
    m_pShield = new CShield(vec3(-45.0f, 7.0f, 45.0f), m_pRenderer);
    m_pSceneMgr->add(m_pShield);
    m_pCannon = new CCannon(vec3(33.9f, 33.9f, -34.0f), m_pRenderer, m_pSceneMgr);
    m_pSceneMgr->add(m_pCannon);
    
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
        // Clear framebuffre
        m_pRenderer->clear();
        
        // Render all objects
        m_pSceneMgr->render();
        
        // Switch buffers and do additional stuff
        m_pRenderer->run();
        
        // new frame has been rendered
        m_pFpsCounter->onFrame();
        
        // Update animation state
        updateAnimation();
    }
}

void CApp::onFpsChange()
{
    // Update window title
    char szBuf[256];
    sprintf(szBuf, "Artillery Animation - FPS: %.1f (max. %.1f)", m_pFpsCounter->getFps(), m_pFpsCounter->getMaxFps());
    m_pRenderer->setTitle(szBuf);
}

void CApp::updateAnimation()
{
    float dt = glfwGetTime() - m_StateChangeTime;
    
    switch(m_State)
    {
        case IDLE:
        {
            if(dt < 3.0f)
                return;
            
            std::pair<float, float> RangeX(0.0f, 8.0f);
            std::pair<float, float> RangeY(-5.0f, 5.0f);
            float RotX = RangeX.first + (rand() % 30000) / 30000.0f * (RangeX.second - RangeX.first);
            float RotY = RangeY.first + (rand() % 30000) / 30000.0f * (RangeY.second - RangeY.first);
            m_pCannon->takeAim(RotX, RotY);
            m_State = AIMING;
            break;
        }
        case AIMING:
        {
            if(!m_pCannon->hasRotationFinished())
                return;
            
            m_State = PAUSE_BEFORE_FIRE;
            break;
        }
        case PAUSE_BEFORE_FIRE:
        {
            if(dt < 2.0f)
                return;
            
            m_pCannon->fire();
            m_State = FIRE;
            break;
        }
        case FIRE:
        {
            if(!m_pCannon->hasFireFinished())
                return;
            
            m_State = IDLE;
            break;
        }
    }
    
    m_StateChangeTime = glfwGetTime();
}
