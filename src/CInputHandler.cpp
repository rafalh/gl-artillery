#include "stdafx.h"
#include "CInputHandler.h"
#include "CRenderer.h"
#include "CCamera.h"

CInputHandler::CInputHandler(CRenderer *pRenderer):
    m_pRenderer(pRenderer), m_pCamera(nullptr) {}

void CInputHandler::onKey(int Key, int Action)
{
    switch(Key)
    {
        case GLFW_KEY_ESCAPE:
            if(Action == GLFW_PRESS)
                m_pRenderer->close();
            break;
        
        case GLFW_KEY_F1:
            if(Action == GLFW_PRESS)
                m_pRenderer->toggleWireframe();
            break;
        
        case GLFW_KEY_W:
        case GLFW_KEY_UP:
            m_pCamera->setControlState(CCamera::CTRL_FORWARD, Action != GLFW_RELEASE);
            break;

        case GLFW_KEY_S:
        case GLFW_KEY_DOWN:
            m_pCamera->setControlState(CCamera::CTRL_BACK, Action != GLFW_RELEASE);
            break;

        case GLFW_KEY_D:
        case GLFW_KEY_RIGHT:
            m_pCamera->setControlState(CCamera::CTRL_RIGHT, Action != GLFW_RELEASE);
            break;

        case GLFW_KEY_A:
        case GLFW_KEY_LEFT:
            m_pCamera->setControlState(CCamera::CTRL_LEFT, Action != GLFW_RELEASE);
            break;

        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            m_pCamera->setSpeed(Action != GLFW_RELEASE ? 8.0f : 1.0f);
            break;
    }
}

void CInputHandler::onMouseMove(float dx, float dy)
{
    // Check if there is any camera
    if(!m_pCamera) return;
    
    // Calculate delta time
    static double LastFrame = glfwGetTime();
    double Now = glfwGetTime();
    float dt = static_cast<float>(Now - LastFrame);
    LastFrame = Now;
    
    // Rotate camera
    m_pCamera->rotate(dx, dy);
    m_pCamera->update(dt);
}
