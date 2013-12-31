#ifndef CSCENEMGR_H
#define CSCENEMGR_H

#include "CSceneNode.h"
#include "CCamera.h"

class CRenderer;

class CSceneMgr
{
    public:
        CSceneMgr(CRenderer *pRenderer);
        ~CSceneMgr();
        
        void add(CSceneNode *pNode)
        {
            if(pNode->isTransparent())
                m_TransparentNodes.push_back(pNode);
            else
                m_Nodes.push_back(pNode);
        }
        
        void setCamera(CCamera *pCamera)
        {
            m_pCamera = pCamera;
        }
        
        void setSkybox(CSceneNode *pSkybox)
        {
            m_pSkybox = pSkybox;
        }
        
        void render();
    
    private:
        std::vector<CSceneNode*> m_Nodes;
        std::vector<CSceneNode*> m_TransparentNodes;
        CSceneNode *m_pSkybox;
        CCamera *m_pCamera;
        CRenderer *m_pRenderer;
        GLuint m_SkyboxProgram;
};

#endif // CSCENEMGR_H
