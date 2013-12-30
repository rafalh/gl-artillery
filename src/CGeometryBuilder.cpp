#include "CGeometryBuilder.h"

CGeometryBuilder::CGeometryBuilder()
{
    
}

void CGeometryBuilder::addQuad(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, const glm::vec3 &v4)
{
    unsigned Idx0 = m_Vertices.size();
    SVertex v;
    v.Normal = glm::normalize(glm::cross(v3 - v2, v1 - v2));
    v.Clr = 0xA0A0A0;
    v.Pos = v1;
    m_Vertices.push_back(v);
    v.Pos = v2;
    m_Vertices.push_back(v);
    v.Pos = v3;
    m_Vertices.push_back(v);
    v.Pos = v4;
    m_Vertices.push_back(v);
    
    m_Indices.push_back(Idx0 + 0);
    m_Indices.push_back(Idx0 + 1);
    m_Indices.push_back(Idx0 + 2);
    
    m_Indices.push_back(Idx0 + 0);
    m_Indices.push_back(Idx0 + 2);
    m_Indices.push_back(Idx0 + 3);
}

void CGeometryBuilder::addBox(const SVertex Vertices[8])
{
    //unsigned Idx0 = m_Vertices.size();
    //for(const SVertex &v: Vertices)
    //    m_Vertices.push_back(v);
}
