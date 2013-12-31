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
        
        bool isTransparent() const
        {
            return false;//true;
        }
        
    private:
        CRenderer *m_pRenderer;
        CMesh *m_pMesh;
        glm::mat4 m_Transform;
        GLuint m_Program;
};

#endif // CSHIELD_H
