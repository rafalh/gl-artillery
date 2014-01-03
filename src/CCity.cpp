#include "CCity.h"
#include "CRenderer.h"
#include "CMesh.h"
#include "CGeometryBuilder.h"
#include "CTextureMgr.h"

using namespace glm;

CCity::CCity(const glm::vec3 &Pos, CRenderer *pRenderer):
    m_pMesh(nullptr), m_pRenderer(pRenderer)
{
    m_Transform = translate(mat4(), Pos);
}

CCity::~CCity()
{
    delete m_pMesh;
}

void CCity::build()
{
    vec3 MinPos(-10.0f, -5.0f, -10.0f);
	vec3 MaxPos(10.0f, 5.0f, 10.0f);
    unsigned CountX = 10, CountZ = 10;
    
    CGeometryBuilder GeomBuilder;
    GeomBuilder.setColor(0xA0A0A0);
    
    float SizeX = (MaxPos.x - MinPos.x) / CountX * (2.0f/3.0f);
    float SizeZ = (MaxPos.z - MinPos.z) / CountZ * (2.0f/3.0f);
    float Margin = (MaxPos.x - MinPos.x) / CountX * (1.0f/6.0f);
    
    for(unsigned z = 0; z < CountZ; ++z)
        for(unsigned x = 0; x < CountX; ++x)
        {
            if(rand() % 2 == 0) continue;
            
            float SizeY = (10 + (rand() % 91)) / 100.f * (MaxPos.y - MinPos.y);
            float WorldX = MinPos.x + x * (2*Margin + SizeX);
            float WorldZ = MinPos.z + z * (2*Margin + SizeZ);
            
            addBuilding(vec3(WorldX, MinPos.y, WorldZ), vec3(WorldX + SizeX, MinPos.y + SizeY, WorldZ + SizeZ), GeomBuilder);
        }
    
    m_pMesh = GeomBuilder.createMesh();
}

void CCity::addBuilding(const glm::vec3 &Min, const glm::vec3 &Max, CGeometryBuilder &Geometry)
{
    vec3 Pos1(Min.x, Min.y, Min.z);
    vec3 Pos2(Min.x, Min.y, Max.z);
    vec3 Pos3(Max.x, Min.y, Max.z);
    vec3 Pos4(Max.x, Min.y, Min.z);
    vec3 Pos5(Min.x, Max.y, Min.z);
    vec3 Pos6(Min.x, Max.y, Max.z);
    vec3 Pos7(Max.x, Max.y, Max.z);
    vec3 Pos8(Max.x, Max.y, Min.z);
    Geometry.addQuad(Pos1, Pos2, Pos6, Pos5);
    Geometry.addQuad(Pos2, Pos3, Pos7, Pos6);
    Geometry.addQuad(Pos3, Pos4, Pos8, Pos7);
    Geometry.addQuad(Pos4, Pos1, Pos5, Pos8);
    Geometry.addQuad(Pos5, Pos6, Pos7, Pos8);
    assert(Max.y > Min.y);
}

void CCity::render()
{
    if(!m_pMesh)
        build();
    
    m_pRenderer->setProgramUniform("MaterialAmbientColor", vec3(0.2f, 0.2f, 0.2f));
    m_pRenderer->setProgramUniform("MaterialDiffuseColor", vec3(1.0f, 1.0f, 1.0f));
    m_pRenderer->setProgramUniform("MaterialSpecularColor", vec3(0.2f, 0.2f, 0.2f));
    m_pRenderer->setProgramUniform("MaterialShininess", 8.0f);
    
    m_pRenderer->setModelTransform(m_Transform);
    m_pRenderer->setTexture(CTextureMgr::getInstance().get("textures/concrete.jpg"));
    m_pMesh->render();
}
