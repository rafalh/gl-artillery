#ifndef CGEOMETRYBUILDER_H
#define CGEOMETRYBUILDER_H

#include "SVertex.h"

class CGeometryBuilder
{
    public:
        CGeometryBuilder();
        void addQuad(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, const glm::vec3 &v4);
        void addBox(const SVertex Vertices[8]);
        void addIcosphere(unsigned n = 0);
        
        void setColor(uint32_t Color)
        {
            m_Color = Color;
        }
        
        const std::vector<SVertex> &getVertices() const
        {
            return m_Vertices;
        }
        
        const std::vector<uint16_t> &getIndices() const
        {
            return m_Indices;
        }
    
    private:
        std::vector<SVertex> m_Vertices;
        std::vector<uint16_t> m_Indices;
        uint32_t m_Color;
        
        void addFace(unsigned Offset, unsigned Index1, unsigned Index2, unsigned Index3);
        unsigned addSphereVertex(const glm::vec3 &Pos);
        unsigned getIcosphereMiddlePoint(unsigned Index1, unsigned Index2, std::unordered_map<unsigned, unsigned> &MiddlePoints);
};

#endif // CGEOMETRYBUILDER_H
