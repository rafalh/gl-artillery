#ifndef CMISSILE_H
#define CMISSILE_H

#include "CSceneNode.h"
#include "SMaterial.h"

class CRenderer;
class CMesh;

class CMissile: public CSceneNode
{
    public:
        CMissile(const glm::vec3 &Pos, const glm::vec3 &Vel, CRenderer *pRenderer);
        ~CMissile();
        void render();
    
    private:
        CRenderer *m_pRenderer;
        CMesh *m_pMesh;
        glm::vec3 m_Pos, m_Vel;
        SMaterial m_Material;
        float m_StartTime;
};

#endif // CMISSILE_H
