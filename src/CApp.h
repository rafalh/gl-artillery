#ifndef CAPP_H
#define CAPP_H

#include "CFpsCounter.h"

class CRenderer;
class CSceneMgr;

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
};

#endif // CAPP_H
