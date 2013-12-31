#ifndef CCANNON_H
#define CCANNON_H

#include "CSceneNode.h"

class CRenderer;
class CMesh;

class CCannon: public CSceneNode
{
    public:
        CCannon(CRenderer *pRenderer);
        ~CCannon();
        void render();
        
    private:
        CRenderer *m_pRenderer;
        CMesh *m_pMesh;
        glm::mat4 m_Transform;
};

#endif // CCANNON_H
