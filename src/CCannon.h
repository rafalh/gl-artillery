#ifndef CCANNON_H
#define CCANNON_H

#include "CSceneNode.h"
#include "CMesh.h"

class CRenderer;
class CGeometryBuilder;

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
        glm::mat4 m_Transform;
        float m_ShotTime;
        const float SHOT_TIME = 2.0f;
        CMesh::SOffsetSize m_CannonBase, m_BarrelBase, m_FrontGun, m_BackGun, m_RotatorX, m_RotatorY;
        
        void prepareBase(CGeometryBuilder &Builder);
        void prepareLauncher(CGeometryBuilder &Builder);
};

#endif // CCANNON_H
