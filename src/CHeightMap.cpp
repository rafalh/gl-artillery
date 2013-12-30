#include "CHeightMap.h"
#include "utils.h"
#include "CRenderer.h"
#include "SVertex.h"
#include "CImage.h"
#include "CTextureMgr.h"
#include "CMesh.h"

CHeightMap::CHeightMap(CRenderer *pRenderer):
    m_pMesh(nullptr),
    m_Texture(0),
    m_pRenderer(pRenderer) {}

CHeightMap::~CHeightMap()
{
    delete m_pMesh;
}

void CHeightMap::build()
{
	glm::vec3 MinPos(-100.0f, -11.0f, -100.0f);
	glm::vec3 MaxPos(100.0f, -1.0f, 100.0f);

    CImage Img("textures/hmap5.png");
    int w = Img.getWidth();
    int h = Img.getHeight();

    m_pMesh = new CMesh;

    unsigned VerticesCount = w * h;
    SVertex *pVertices = new SVertex[VerticesCount];
    for(int y = 0; y < h; ++y)
        for(int x = 0; x < w; ++x)
        {
            SVertex &Vert = pVertices[y * w + x];
            RGBQUAD rgb = Img.getPixel(x, y);
            
            float vx = MinPos.x + x * (MaxPos.x - MinPos.x) / (w - 1);
            float vy = MinPos.y + (rgb.rgbRed / 255.0f) * (MaxPos.y - MinPos.y);
            float vz = MinPos.z + y * (MaxPos.z - MinPos.z) / (h - 1);
            Vert.Pos = glm::vec3(vx, vy, vz);
            Vert.UV = glm::vec2(vx, vz);
            Vert.Clr = 0xFFFFFF;
        }
    for(int y = 0; y < h; ++y)
        for(int x = 0; x < w; ++x)
        {
            if(x > 0 && x < w - 1 && y > 0 && y < h - 1)
            {
                glm::vec3 &Pos = pVertices[y * w + x].Pos;
                glm::vec3 &PosL = pVertices[y * w + x - 1].Pos;
                glm::vec3 &PosR = pVertices[y * w + x + 1].Pos;
                glm::vec3 &PosT = pVertices[(y - 1) * w + x].Pos;
                glm::vec3 &PosB = pVertices[(y + 1) * w + x].Pos;
                glm::vec3 &PosLT = pVertices[(y - 1) * w + x - 1].Pos;
                glm::vec3 &PosRB = pVertices[(y + 1) * w + x + 1].Pos;
                glm::vec3 n1 = glm::normalize(glm::cross(PosLT - Pos, PosL - Pos));
                assert(n1.y >= 0.0f);
                glm::vec3 n2 = glm::normalize(glm::cross(PosL - Pos, PosB - Pos));
                assert(n2.y >= 0.0f);
                glm::vec3 n3 = glm::normalize(glm::cross(PosB - Pos, PosRB - Pos));
                assert(n3.y >= 0.0f);
                glm::vec3 n4 = glm::normalize(glm::cross(PosRB - Pos, PosR - Pos));
                assert(n4.y >= 0.0f);
                glm::vec3 n5 = glm::normalize(glm::cross(PosR - Pos, PosT - Pos));
                assert(n4.y >= 0.0f);
                glm::vec3 n6 = glm::normalize(glm::cross(PosT - Pos, PosLT - Pos));
                assert(n4.y >= 0.0f);
                
                pVertices[y * w + x].Normal = (n1 + n2 + n3 + n4 + n5 + n6) / 6.0f;
            }
            else
                pVertices[y * w + x].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        }
    m_pMesh->setVertices(pVertices, VerticesCount);
    delete[] pVertices;
    
    unsigned IndicesCount = (Img.getWidth() - 1) * (Img.getHeight() - 1) * 2 * 3;
    uint32_t *pIndices = new uint32_t[IndicesCount];
    unsigned i = 0;
    for(int y = 0; y < h - 1; ++y)
        for(int x = 0; x < w - 1; ++x)
        {
            pIndices[i++] = y * w + x;
            pIndices[i++] = (y + 1) * w + x;
            pIndices[i++] = (y + 1) * w + x + 1;
            
            pIndices[i++] = y * w + x;
            pIndices[i++] = (y + 1) * w + x + 1;
            pIndices[i++] = y * w + x + 1;
        }
    m_pMesh->setIndices(pIndices, IndicesCount);
    delete[] pIndices;
    
    m_Texture = CTextureMgr::getInstance().get("textures/desert.jpg");
}

void CHeightMap::render()
{
    if(!m_pMesh)
        build();
    
    m_pRenderer->setModelTransform(glm::mat4(1.0f));
    
    m_pRenderer->setTexture(m_Texture);
    
    m_pMesh->render();
}
