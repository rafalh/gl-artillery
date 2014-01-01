#include "CGeometryBuilder.h"
#include "CLogger.h"

const float PI = 3.141592;

CGeometryBuilder::CGeometryBuilder():
    m_Color(0xFFFFFFFF) {}

void CGeometryBuilder::addQuad(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, const glm::vec3 &v4)
{
    unsigned BaseIdx = m_Vertices.size();
    glm::vec3 Normal = glm::normalize(glm::cross(v3 - v2, v1 - v2));
    m_Vertices.push_back(SVertex(v1, Normal, glm::vec2(0.0f, 0.0f), m_Color));
    m_Vertices.push_back(SVertex(v2, Normal, glm::vec2(1.0f, 0.0f), m_Color));
    m_Vertices.push_back(SVertex(v3, Normal, glm::vec2(1.0f, 1.0f), m_Color));
    m_Vertices.push_back(SVertex(v4, Normal, glm::vec2(0.0f, 1.0f), m_Color));
    
    m_Indices.push_back(BaseIdx + 0);
    m_Indices.push_back(BaseIdx + 1);
    m_Indices.push_back(BaseIdx + 2);
    
    m_Indices.push_back(BaseIdx + 0);
    m_Indices.push_back(BaseIdx + 2);
    m_Indices.push_back(BaseIdx + 3);
}

// Based on http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
void CGeometryBuilder::addIcosphere(unsigned n)
{
    unsigned BaseIdx = m_Vertices.size();
    float t = (1.0f + sqrtf(5.0f)) / 2.0f;

    m_Vertices.reserve(m_Vertices.size() + 12 * pow(2.5f, n));

    addSphereVertex(glm::normalize(glm::vec3(-1.0f,  t,  0.0f)));
    addSphereVertex(glm::normalize(glm::vec3( 1.0f,  t,  0.0f)));
    addSphereVertex(glm::normalize(glm::vec3(-1.0f, -t,  0.0f)));
    addSphereVertex(glm::normalize(glm::vec3( 1.0f, -t,  0.0f)));
    
    addSphereVertex(glm::normalize(glm::vec3( 0.0f, -1.0f,  t)));
    addSphereVertex(glm::normalize(glm::vec3( 0.0f,  1.0f,  t)));
    addSphereVertex(glm::normalize(glm::vec3( 0.0f, -1.0f, -t)));
    addSphereVertex(glm::normalize(glm::vec3( 0.0f,  1.0f, -t)));
    
    addSphereVertex(glm::normalize(glm::vec3( t,  0.0f, -1.0f)));
    addSphereVertex(glm::normalize(glm::vec3( t,  0.0f,  1.0f)));
    addSphereVertex(glm::normalize(glm::vec3(-t,  0.0f, -1.0f)));
    addSphereVertex(glm::normalize(glm::vec3(-t,  0.0f,  1.0f)));
    
    std::vector<unsigned> Indices = {
        // 5 faces around point 0
        0, 11, 5,
    	0, 5, 1,
    	0, 1, 7,
    	0, 7, 10,
    	0, 10, 11,
		
		// 5 adjacent faces
    	1, 5, 9,
    	5, 11, 4,
    	11, 10, 2,
    	10, 7, 6,
    	7, 1, 8,
        
		// 5 faces around point 3
    	3, 9, 4,
    	3, 4, 2,
    	3, 2, 6,
    	3, 6, 8,
    	3, 8, 9,
		
		// 5 adjacent faces
    	4, 9, 5,
    	2, 4, 11,
    	6, 2, 10,
    	8, 6, 7,
    	9, 8, 1,
    };
    
    std::unordered_map<unsigned, unsigned> MiddlePoints;
    
    while(n > 0)
    {
        std::vector<unsigned> NewIndices;
        
        for(unsigned i = 0; i < Indices.size(); i += 3)
        {
            // replace triangle by 4 triangles
            unsigned IdxA = Indices[i + 0];
            unsigned IdxB = Indices[i + 1];
            unsigned IdxC = Indices[i + 2];
            
            unsigned IdxAB = getIcosphereMiddlePoint(BaseIdx + IdxA, BaseIdx + IdxB, MiddlePoints) - BaseIdx;
            unsigned IdxBC = getIcosphereMiddlePoint(BaseIdx + IdxB, BaseIdx + IdxC, MiddlePoints) - BaseIdx;
            unsigned IdxCA = getIcosphereMiddlePoint(BaseIdx + IdxC, BaseIdx + IdxA, MiddlePoints) - BaseIdx;
            
            unsigned Temp[] = {
                IdxA, IdxAB, IdxCA,
                IdxB, IdxBC, IdxAB,
                IdxC, IdxCA, IdxBC,
                IdxAB, IdxBC, IdxCA,
            };
            NewIndices.insert(NewIndices.end(), Temp, Temp + 12);
        }
        
        Indices.swap(NewIndices);
        --n;
    }
    
    for(unsigned Idx: Indices)
    {
        m_Indices.push_back(BaseIdx + Idx);
        assert(BaseIdx + Idx < 65000);
    }
    
    CLogger::getInstance().info("Icosphere %u has %u vertices\n", n, m_Vertices.size() - BaseIdx);
}

unsigned CGeometryBuilder::getIcosphereMiddlePoint(unsigned Index1, unsigned Index2, std::unordered_map<unsigned, unsigned> &MiddlePoints)
{
    unsigned Hash = std::min(Index1, Index2) << 16 | std::max(Index1, Index2);
    auto it = MiddlePoints.find(Hash);
    if(it != MiddlePoints.end())
        return it->second;
    
    const SVertex &v1 = m_Vertices[Index1];
    const SVertex &v2 = m_Vertices[Index2];
    glm::vec3 Pos = glm::normalize(v1.Pos + v2.Pos);
    unsigned Idx = addSphereVertex(Pos);
    MiddlePoints.insert(std::make_pair(Hash, Idx));
    return Idx;
}

unsigned CGeometryBuilder::addSphereVertex(const glm::vec3 &Pos)
{
    glm::vec2 uv;
    uv.x = atan2f(Pos.x, Pos.z) / (2.0f * PI);
    uv.y = asinf(Pos.y) / PI + 0.5f;
    SVertex v(Pos, Pos, uv, m_Color);
    //if(Pos.x == 0.0f)
    //    v.Clr = 0xFFFF0000;
    m_Vertices.push_back(v);
    return m_Vertices.size() - 1;
}

void CGeometryBuilder::addFace(unsigned Offset, unsigned Index1, unsigned Index2, unsigned Index3)
{
    m_Indices.push_back(Offset + Index1);
    m_Indices.push_back(Offset + Index2);
    m_Indices.push_back(Offset + Index3);
}

void CGeometryBuilder::addCylinder(float Radius, bool Inside, unsigned Segments)
{
    unsigned PrevIdx = m_Vertices.size() + 2 * (Segments - 1);
    
    for(unsigned i = 0; i < Segments; ++i)
    {
        float Angle = (i / (float)Segments) * 2.0f * PI;
        glm::vec3 Pos(cosf(Angle), 0.0f, sinf(Angle));
        glm::vec3 Normal = Inside ? -Pos : Pos;
        Pos *= Radius;
        
        unsigned CurIdx = m_Vertices.size();
        m_Vertices.push_back(SVertex(glm::vec3(Pos.x, -1.0f, Pos.z), Normal, glm::vec2(), m_Color));
        m_Vertices.push_back(SVertex(glm::vec3(Pos.x,  1.0f, Pos.z), Normal, glm::vec2(), m_Color));
        
        if(!Inside)
        {
            m_Indices.push_back(PrevIdx);
            m_Indices.push_back(PrevIdx + 1);
            m_Indices.push_back(CurIdx + 1);
            
            m_Indices.push_back(PrevIdx);
            m_Indices.push_back(CurIdx + 1);
            m_Indices.push_back(CurIdx);
        }
        else
        {
            m_Indices.push_back(PrevIdx + 1);
            m_Indices.push_back(PrevIdx);
            m_Indices.push_back(CurIdx + 1);
            
            m_Indices.push_back(PrevIdx);
            m_Indices.push_back(CurIdx);
            m_Indices.push_back(CurIdx + 1);
        }
        
        PrevIdx = CurIdx;
    }
}

void CGeometryBuilder::addTube(float RadiusIn, float RadiusOut, unsigned Segments)
{
    addCylinder(RadiusIn, true, Segments);
    addCylinder(RadiusOut, false, Segments);
    
    glm::vec3 NormalTop(0.0f, 1.0f, 0.0f);
    glm::vec3 NormalBottom(0.0f, -1.0f, 0.0f);
    
    unsigned PrevIdx = m_Vertices.size() + 4 * (Segments - 1);
    
    for(unsigned i = 0; i < Segments; ++i)
    {
        float Angle = (i / (float)Segments) * 2.0f * PI;
        glm::vec3 Pos(cosf(Angle), 0.0f, sinf(Angle));
        glm::vec3 PosTopIn(Pos.x * RadiusIn, 1.0f, Pos.z * RadiusIn);
        glm::vec3 PosTopOut(Pos.x * RadiusOut, 1.0f, Pos.z * RadiusOut);
        glm::vec3 PosBottomIn(Pos.x * RadiusIn, -1.0f, Pos.z * RadiusIn);
        glm::vec3 PosBottomOut(Pos.x * RadiusOut, -1.0f, Pos.z * RadiusOut);
        
        unsigned CurIdx = m_Vertices.size();
        m_Vertices.push_back(SVertex(PosTopIn, NormalTop, glm::vec2(), m_Color));
        m_Vertices.push_back(SVertex(PosTopOut, NormalTop, glm::vec2(), m_Color));
        m_Vertices.push_back(SVertex(PosBottomIn, NormalBottom, glm::vec2(), m_Color));
        m_Vertices.push_back(SVertex(PosBottomOut, NormalBottom, glm::vec2(), m_Color));
        
        // Top
        m_Indices.push_back(CurIdx + 0);
        m_Indices.push_back(CurIdx + 1);
        m_Indices.push_back(PrevIdx + 1);
        
        m_Indices.push_back(CurIdx + 0);
        m_Indices.push_back(PrevIdx + 1);
        m_Indices.push_back(PrevIdx + 0);
        
        // Bottom
        m_Indices.push_back(CurIdx + 3);
        m_Indices.push_back(CurIdx + 2);
        m_Indices.push_back(PrevIdx + 3);
        
        m_Indices.push_back(CurIdx + 2);
        m_Indices.push_back(PrevIdx + 2);
        m_Indices.push_back(PrevIdx + 3);
        
        PrevIdx = CurIdx;
    }
}
