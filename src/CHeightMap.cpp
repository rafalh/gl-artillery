#include "CHeightMap.h"
#include "utils.h"
#include "CRenderer.h"
#include "SVertex.h"
#include "CImage.h"
#include "CTextureMgr.h"
#include "CMesh.h"

using namespace glm;

CHeightMap::CHeightMap(const std::string &Path, CRenderer *pRenderer):
    m_pMesh(nullptr),
    m_pRenderer(pRenderer)
{
    m_pImage = new CImage(Path.c_str());
    m_pImage->convertToGrayscale16();
}

CHeightMap::~CHeightMap()
{
    delete m_pMesh;
    delete m_pImage;
}

void CHeightMap::build()
{
    if(!m_pImage) return;
    
    float StartTime = glfwGetTime();
    
	vec3 MinPos(-200.0f, 0.0f, -200.0f);
	vec3 MaxPos(200.0f, 80.0f, 200.0f);
    
    int w = m_pImage->getWidth();
    int h = m_pImage->getHeight();
    

    m_pMesh = new CMesh;
    
    unsigned VerticesCount = w * h;
    SVertex *pVertices = new SVertex[VerticesCount];
    for(int y = 0; y < h; ++y)
    {
        WORD *pScanLine = reinterpret_cast<WORD*>(m_pImage->getScanLine(y));
        
        for(int x = 0; x < w; ++x)
        {
            SVertex &Vert = pVertices[y * w + x];
            WORD Val = pScanLine[x];
            
            float vx = MinPos.x + x * (MaxPos.x - MinPos.x) / (w - 1);
            float vy = MinPos.y + (Val / 65535.0f) * (MaxPos.y - MinPos.y);
            float vz = MinPos.z + y * (MaxPos.z - MinPos.z) / (h - 1);
            Vert.Pos = vec3(vx, vy, vz);
            Vert.UV = vec2(vx / 5.0f, vz / 5.0f);
            Vert.Clr = 0xFFFFFF;
        }
    }
    for(int y = 0; y < h; ++y)
        for(int x = 0; x < w; ++x)
        {
            if(x > 0 && x < w - 1 && y > 0 && y < h - 1)
            {
                const vec3 &Pos = pVertices[y * w + x].Pos;
                const vec3 &PosL = pVertices[y * w + x - 1].Pos;
                const vec3 &PosR = pVertices[y * w + x + 1].Pos;
                const vec3 &PosT = pVertices[(y - 1) * w + x].Pos;
                const vec3 &PosB = pVertices[(y + 1) * w + x].Pos;
                const vec3 &PosLT = pVertices[(y - 1) * w + x - 1].Pos;
                const vec3 &PosRB = pVertices[(y + 1) * w + x + 1].Pos;
                vec3 n1 = normalize(cross(PosLT - Pos, PosL - Pos));
                //assert(n1.y >= 0.0f);
                vec3 n2 = normalize(cross(PosL - Pos, PosB - Pos));
                //assert(n2.y >= 0.0f);
                vec3 n3 = normalize(cross(PosB - Pos, PosRB - Pos));
                //assert(n3.y >= 0.0f);
                vec3 n4 = normalize(cross(PosRB - Pos, PosR - Pos));
                //assert(n4.y >= 0.0f);
                vec3 n5 = normalize(cross(PosR - Pos, PosT - Pos));
                //assert(n4.y >= 0.0f);
                vec3 n6 = normalize(cross(PosT - Pos, PosLT - Pos));
                //assert(n4.y >= 0.0f);
                
                pVertices[y * w + x].Normal = normalize((n1 + n2 + n3 + n4 + n5 + n6) / 6.0f);
            }
            else
                pVertices[y * w + x].Normal = vec3(0.0f, 1.0f, 0.0f);
        }
    m_pMesh->setVertices(pVertices, VerticesCount);
    delete[] pVertices;
    
    unsigned IndicesCount = (w - 1) * (h - 1) * 2 * 3;
	std::uint32_t *pIndices = new std::uint32_t[IndicesCount];
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
    
    delete m_pImage;
    m_pImage = nullptr;
    
    m_Material.Texture = CTextureMgr::getInstance().get("textures/desert.jpg");
    m_Material.AmbientColor = vec3(0.2f, 0.2f, 0.2f);
    m_Material.DiffuseColor = vec3(1.0f, 1.0f, 1.0f);
    m_Material.SpecularColor = vec3(0.1f, 0.1f, 0.1f);
    m_Material.Shininess = 8.0f;
    
    CLogger::getInstance().info("Heightmap %dx%d built in %.2f seconds\n", w, h, glfwGetTime() - StartTime);
}

void CHeightMap::render()
{
    if(!m_pMesh)
        build();
    
    m_pRenderer->setModelTransform(glm::mat4(1.0f));
    m_pRenderer->setMaterial(m_Material);
    m_pMesh->render();
}
