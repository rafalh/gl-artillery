#ifndef CAPP_H
#define CAPP_H

#include "CFpsCounter.h"

class CRenderer;
class CSceneMgr;
class CCannon;
class CShield;

class CApp: public CFpsCounter::IObserver
{
    public:
        CApp();
        ~CApp();
        void start();
        void onFpsChange();
        
    private:
        CRenderer *m_pRenderer;
        CSceneMgr *m_pSceneMgr;
        CFpsCounter *m_pFpsCounter;
        CShield *m_pShield;
        CCannon *m_pCannon;
        float m_StateChangeTime;
        enum {IDLE, AIMING, PAUSE_BEFORE_FIRE, FIRE} m_State;
        
        void updateAnimation();
};

#endif // CAPP_H
