#ifndef CHEIGHTMAP_H
#define CHEIGHTMAP_H

#include "CSceneNode.h"

class CRenderer;
class CMesh;

class CHeightMap: public CSceneNode
{
    public:
        CHeightMap(CRenderer *pRenderer);
        virtual ~CHeightMap();
        void build();
        void render();
        
    private:
        CMesh *m_pMesh;
        GLuint m_Texture;
        CRenderer *m_pRenderer;
};

#endif // CHEIGHTMAP_H
