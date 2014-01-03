#ifndef CHEIGHTMAP_H
#define CHEIGHTMAP_H

#include "CSceneNode.h"
#include "SMaterial.h"

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
        SMaterial m_Material;
        CRenderer *m_pRenderer;
        CImage *m_pImage;
};

#endif // CHEIGHTMAP_H
