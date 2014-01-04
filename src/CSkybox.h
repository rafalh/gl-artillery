#ifndef CSKYBOX_H
#define CSKYBOX_H

#include "CSceneNode.h"
#include "CMesh.h"
#include "SMaterial.h"

class CRenderer;

class CSkybox: public CSceneNode
{
    public:
        CSkybox(const char *pszFilenamePattern, CRenderer *pRenderer);
        virtual ~CSkybox();
        void render();
    
    private:
        CRenderer *m_pRenderer;
        GLuint m_Textures[6];
        SMaterial m_Material;
        CMesh *m_pMesh;
};

#endif // CSKYBOX_H
