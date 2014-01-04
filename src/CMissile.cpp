#include "stdafx.h"
#include "CMissile.h"
#include "CGeometryBuilder.h"
#include "CRenderer.h"

using namespace glm;

CMissile::CMissile(CRenderer *pRenderer):
    m_pRenderer(pRenderer), m_pMesh(nullptr)
{
    CGeometryBuilder Builder;
    Builder.setTransform(scale(mat4(), vec3(0.4f, 0.4f, 0.4f)));
    Builder.addIcosphere(2);
    m_pMesh = Builder.createMesh();
    
    m_Material.AmbientColor = vec3(0.9f, 0.6f, 0.3f);
    m_Material.DiffuseColor = vec3(0.4f, 0.2f, 0.0f);
    
    m_StartTime = glfwGetTime();
}

CMissile::~CMissile()
{
    delete m_pMesh;
}

void CMissile::animate()
{
    const float G = 9.8f;
    float dt = glfwGetTime() - m_StartTime;
    m_Pos = m_StartPos + dt * m_Vel + vec3(0.0f, -G, 0.0f) * (dt*dt) / 2.0f;
    
    m_pRenderer->setLightColor(vec3(800.0f, 400.0f, 0.0f));
    m_pRenderer->setLightPos(m_Pos);
}

void CMissile::render()
{
    mat4 PosTrans = translate(mat4(), m_Pos);
    m_pRenderer->setModelTransform(PosTrans);
    m_pRenderer->setMaterial(m_Material);
    m_pMesh->render();
}
