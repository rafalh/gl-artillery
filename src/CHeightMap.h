#ifndef CHEIGHTMAP_H
#define CHEIGHTMAP_H

#include "CSceneNode.h"

class CRenderer;
class CMesh;
class CImage;

class CHeightMap: public CSceneNode
{
    public:
        CHeightMap(const std::string &Path, CRenderer *pRenderer);
        ~CHeightMap();
        void build();
        void render();
        
    private:
        CMesh *m_pMesh;
        GLuint m_Texture;
        CRenderer *m_pRenderer;
        CImage *m_pImage;
};

#endif // CHEIGHTMAP_H
