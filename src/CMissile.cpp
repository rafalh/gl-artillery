#include "stdafx.h"
#include "CMissile.h"
#include "CGeometryBuilder.h"
#include "CRenderer.h"

using namespace glm;

CMissile::CMissile(const glm::vec3 &Pos, const glm::vec3 &Vel, CRenderer *pRenderer):
    m_pRenderer(pRenderer), m_pMesh(nullptr), m_Pos(Pos), m_Vel(Vel)
{
    CGeometryBuilder Builder;
    Builder.setTransform(scale(mat4(), vec3(0.4f, 0.4f, 0.4f)));
    Builder.addIcosphere(2);
    m_pMesh = Builder.createMesh();
    
    m_Material.AmbientColor = vec3(0.8f, 0.5f, 0.0f);
    m_Material.DiffuseColor = vec3(0.4f, 0.2f, 0.0f);
    
    m_StartTime = glfwGetTime();
}

CMissile::~CMissile()
{
    delete m_pMesh;
}

void CMissile::render()
{
    const float G = 9.8f;
    float dt = glfwGetTime() - m_StartTime;
    vec3 NewPos = m_Pos + dt * m_Vel + vec3(0.0f, -G, 0.0f) * (dt*dt) / 2.0f;
    
    mat4 PosTrans = translate(mat4(), NewPos);
    m_pRenderer->setModelTransform(PosTrans);
    m_pRenderer->setMaterial(m_Material);
    m_pMesh->render();
}
