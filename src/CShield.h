#ifndef CSHIELD_H
#define CSHIELD_H

#include "CSceneNode.h"

class CRenderer;
class CMesh;

class CShield: public CSceneNode
{
    public:
        CShield(const glm::vec3 &Pos, CRenderer *pRenderer);
        ~CShield();
        void render();
        
        bool isTransparent() const
        {
            return true;
        }
        
    private:
        CRenderer *m_pRenderer;
        CMesh *m_pMesh;
        glm::mat4 m_Transform;
};

#endif // CSHIELD_H
