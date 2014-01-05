#ifndef CSHIELD_H
#define CSHIELD_H

#include "CSceneNode.h"

class CRenderer;
class CMesh;
class CMissile;

class CShield: public CSceneNode
{
    public:
        CShield(const glm::vec3 &Pos, CRenderer *pRenderer);
        ~CShield();
        void render();
        bool testCollision(const glm::vec3 RayBegin, const glm::vec3 RayEnd);
        
        bool isTransparent() const
        {
            return true;
        }
        
    private:
        glm::mat4 m_Transform;
        glm::vec3 m_Pos, m_ColPos;
        float m_Radius, m_ColTime;
        CRenderer *m_pRenderer;
        CMesh *m_pMesh;
};

#endif // CSHIELD_H
