#ifndef CCANNON_H
#define CCANNON_H

#include "CSceneNode.h"
#include "CMesh.h"
#include "SMaterial.h"

class CRenderer;
class CGeometryBuilder;
class CMissile;

class CCannon: public CSceneNode
{
    public:
        CCannon(const glm::vec3 &Pos, CRenderer *pRenderer);
        ~CCannon();
        void render();
        void shoot();
        
    private:
        CRenderer *m_pRenderer;
        CMesh *m_pMesh;
        CMissile *m_pMissile;
        glm::mat4 m_Transform, m_BarrelTransform;
        SMaterial m_CannonMaterial;
        SMaterial m_DisabledRingMaterial;
        SMaterial m_EnabledRingMaterial;
        float m_ShotTime;
        float m_AngleX, m_AngleY;
        const float SHOT_TIME = 2.0f;
        CMesh::SOffsetSize m_CannonBase, m_BarrelBase, m_FrontGun, m_BackGun, m_RotatorX, m_RotatorY;
        std::vector<CMesh::SOffsetSize> m_BarrelRings;
        
        void prepareBase(CGeometryBuilder &Builder);
        void prepareLauncher(CGeometryBuilder &Builder);
};

#endif // CCANNON_H
