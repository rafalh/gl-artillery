#ifndef CSCENEMGR_H
#define CSCENEMGR_H

#include "CSceneNode.h"
#include "CCamera.h"

class CRenderer;
class CGlowmap;

class CSceneMgr
{
    public:
        CSceneMgr(CRenderer *pRenderer);
        ~CSceneMgr();
        void add(CSceneNode *pNode);
        void render();
        void buildGlowmap();
        bool testCollision(const glm::vec3 RayBegin, const glm::vec3 RayEnd);
        
        void setCamera(CCamera *pCamera)
        {
            m_pCamera = pCamera;
        }
        
        void setSkybox(CSceneNode *pSkybox)
        {
            m_pSkybox = pSkybox;
        }
        
    private:
        std::vector<CSceneNode*> m_Nodes;
        std::vector<CSceneNode*> m_TransparentNodes;
        CSceneNode *m_pSkybox;
        CCamera *m_pCamera;
        CRenderer *m_pRenderer;
        CGlowmap *m_pGlowmap;
        bool m_bHasEmissiveNodes;
        
};

#endif // CSCENEMGR_H
