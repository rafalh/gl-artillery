#ifndef CINPUTHANDLER_H
#define CINPUTHANDLER_H

class CRenderer;
class CCamera;

class CInputHandler
{
    public:
        CInputHandler(CRenderer *pRenderer);
        
        void onKey(int Key, int Action);
        void onMouseMove(float dx, float dy);
        
        void setCamera(CCamera *pCamera)
        {
            m_pCamera = pCamera;
        }
        
    private:
        CRenderer *m_pRenderer;
        CCamera *m_pCamera;
};

#endif // CINPUTHANDLER_H
