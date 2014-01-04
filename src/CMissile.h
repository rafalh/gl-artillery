#ifndef CMISSILE_H
#define CMISSILE_H

#include "CSceneNode.h"
#include "SMaterial.h"

class CRenderer;
class CMesh;

class CMissile: public CSceneNode
{
    public:
        CMissile(CRenderer *pRenderer);
        ~CMissile();
        void render();
        void animate();
        
        void start(const glm::vec3 &Pos)
        {
            m_StartPos = m_Pos = Pos;
            m_StartTime = glfwGetTime();
        }
        
        void setVelocity(const glm::vec3 &Vel)
        {
            m_Vel = Vel;
        }
    
    private:
        CRenderer *m_pRenderer;
        CMesh *m_pMesh;
        glm::vec3 m_StartPos, m_Pos, m_Vel;
        SMaterial m_Material;
        float m_StartTime;
};

#endif // CMISSILE_H
