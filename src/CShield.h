#ifndef CSHIELD_H
#define CSHIELD_H

#include "CSceneNode.h"

class CRenderer;
class CMesh;

class CShield: public CSceneNode
{
    public:
        CShield(CRenderer *pRenderer);
        ~CShield();
        void render();
        
    private:
        CRenderer *m_pRenderer;
        CMesh *m_pMesh;
};

#endif // CSHIELD_H
