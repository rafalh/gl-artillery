#ifndef CMESH_H
#define CMESH_H

#include "SVertex.h"

class CMesh
{
    public:
        CMesh(bool bDrawNormals = false);
        ~CMesh();
        void setVertices(const SVertex *pVertices, unsigned Count);
        void setIndices(const uint32_t *pIndices, unsigned Count);
        void setIndices(const uint16_t *pIndices, unsigned Count);
        void render();
        void renderPart(unsigned Offset, unsigned Count);
        
    private:
        GLuint m_VertexArray;
        GLuint m_VertexBuf, m_ElementBuf;
        unsigned m_VerticesCount, m_IndicesCount;
        GLuint m_IndexType;
        bool m_bDrawNormals;
};

#endif // CMESH_H
