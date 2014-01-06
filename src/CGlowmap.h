#ifndef CGLOWMAP_H
#define CGLOWMAP_H

class CRenderer;
class CFrameBuffer;
class CMesh;

class CGlowmap
{
    public:
        CGlowmap(CRenderer *pRenderer, int Size = 256);
        ~CGlowmap();
        void begin();
        void end();
        void render();
    
    private:
        CRenderer *m_pRenderer;
        CFrameBuffer *m_pGlowmap, *m_pGlowmap2, *m_pGlowmap3;
        CMesh *m_pQuadMesh;
        int m_Size;
};

#endif // CGLOWMAP_H
