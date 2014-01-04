#ifndef CMISSILE_H
#define CMISSILE_H

#include "CSceneNode.h"
#include "SMaterial.h"

class CRenderer;
class CSceneMgr;
class CMesh;

class CMissile: public CSceneNode
{
    public:
        CMissile(CRenderer *pRenderer, CSceneMgr *pSceneMgr);
        ~CMissile();
        void render();
        void animate();
        
        void start(const glm::vec3 &Pos)
        {
            m_StartPos = m_Pos = Pos;
            m_StartTime = glfwGetTime();
            m_Visible = true;
        }
        
        void setVelocity(const glm::vec3 &Vel)
        {
            m_Vel = Vel;
        }
    
    private:
        CRenderer *m_pRenderer;
        CSceneMgr *m_pSceneMgr;
        CMesh *m_pMesh;
        glm::vec3 m_StartPos, m_Pos, m_Vel;
        SMaterial m_Material;
        float m_StartTime;
        bool m_Visible;
};

#endif // CMISSILE_H
