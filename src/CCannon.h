#ifndef CCANNON_H
#define CCANNON_H

#include "CSceneNode.h"
#include "CMesh.h"
#include "SMaterial.h"

class CRenderer;
class CGeometryBuilder;
class CMissile;
class CSceneMgr;

class CCannon: public CSceneNode
{
    public:
        CCannon(const glm::vec3 &Pos, CRenderer *pRenderer, CSceneMgr *pSceneMgr);
        ~CCannon();
        void render();
        void fire();
        void animate();
        
        void takeAim(float RotX, float RotY)
        {
            m_DestAngleX = RotX;
            m_DestAngleY = RotY;
        }
        
        bool hasRotationFinished() const
        {
            if(fabsf(m_DestAngleX - m_AngleX) < 0.0001f && fabsf(m_DestAngleY - m_AngleY) < 0.0001f)
                return true;
            return false;
        }
        
        bool hasFireFinished() const
        {
            if(m_ShotTime + SHOT_TIME >= glfwGetTime())
                return true;
            return false;
        }
        
    private:
        const float SHOT_TIME = 2.0f;
        
        CRenderer *m_pRenderer;
        CMesh *m_pMesh;
        CMissile *m_pMissile;
        glm::mat4 m_Transform, m_BarrelTransform;
        SMaterial m_CannonMaterial;
        SMaterial m_DisabledRingMaterial;
        SMaterial m_EnabledRingMaterial;
        float m_ShotTime, m_PrevFrameTime;
        float m_DestAngleX, m_DestAngleY;
        float m_AngleX, m_AngleY;
        bool m_MissileVisible;
        CMesh::SOffsetSize m_CannonBase, m_BarrelBase, m_FrontGun, m_BackGun, m_RotatorX, m_RotatorY;
        std::vector<CMesh::SOffsetSize> m_BarrelRings;
        
        void prepareBase(CGeometryBuilder &Builder);
        void prepareLauncher(CGeometryBuilder &Builder);
        
};

#endif // CCANNON_H
