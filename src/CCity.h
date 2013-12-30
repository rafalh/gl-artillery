#ifndef CCITY_H
#define CCITY_H

#include "CSceneNode.h"
#include "SVertex.h"

class CMesh;
class CRenderer;
class CGeometryBuilder;

class CCity: public CSceneNode
{
    public:
        CCity(CRenderer *pRenderer);
        ~CCity();
        void build();
        void render();
    
    private:
        CMesh *m_pMesh;
        CRenderer *m_pRenderer;
        glm::mat4 m_Transform;
        
        void addBuilding(const glm::vec3 &Min, const glm::vec3 &Max, CGeometryBuilder &Geometry);
        
};

#endif // CCITY_H
