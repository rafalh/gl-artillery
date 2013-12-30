#include "stdafx.h"
#include "CFpsCounter.h"

using namespace std;

CFpsCounter::CFpsCounter():
    m_CurrentFps(0.0f), m_MinFps(0.0f), m_MaxFps(0.0f),
    m_Counter(0), m_FirstFrame(true) {}

void CFpsCounter::onFrame()
{
    // Get time in seconds
    double Now = glfwGetTime();
    
    if(m_FirstFrame)
    {
        // Ignore first frame
        m_LastUpdate = Now;
        m_FirstFrame = false;
        return;
    }
    
    // New frame has been rendered
    ++m_Counter;
    
    // Check if it's time to update FPS value
    float dt = static_cast<float>(Now - m_LastUpdate);
    if(dt >= 1.0f && m_Counter > 1)
    {
        // Calculate new FPS calue
        m_CurrentFps = static_cast<float>(m_Counter) / dt;
        m_MinFps = min(m_MinFps, m_CurrentFps);
        m_MaxFps = max(m_MaxFps, m_CurrentFps);
        
        // Zero the counter
        m_Counter = 0;
        m_LastUpdate = Now;
        
        // Notify observers
        for(IObserver *pObs: m_Observers)
            pObs->onFpsChange();
    }
}
