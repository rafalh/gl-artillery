#include "stdafx.h"
#include "CMissile.h"
#include "CGeometryBuilder.h"
#include "CRenderer.h"
#include "CSceneMgr.h"
#include "CLogger.h"

using namespace glm;

CMissile::CMissile(CRenderer *pRenderer, CSceneMgr *pSceneMgr):
    m_pRenderer(pRenderer), m_pSceneMgr(pSceneMgr),
    m_pMesh(nullptr), m_Visible(false)
{
    CGeometryBuilder Builder;
    Builder.setTransform(scale(mat4(), vec3(0.4f, 0.4f, 0.4f)));
    Builder.addIcosphere(2);
    m_pMesh = Builder.createMesh();
    
    m_Material.AmbientColor = vec3(1.0f, 0.7f, 0.4f);
    m_Material.DiffuseColor = vec3(0.1f, 0.1f, 0.0f);
    
    m_StartTime = glfwGetTime();
}

CMissile::~CMissile()
{
    delete m_pMesh;
}

void CMissile::animate()
{
    if(!m_Visible)
        return; // nothing to do
    
    const float G = 9.8f;
    float dt = glfwGetTime() - m_StartTime;
    vec3 NewPos = m_StartPos + dt * m_Vel + vec3(0.0f, -G, 0.0f) * (dt*dt) / 2.0f;
    if(m_pSceneMgr->testCollision(m_Pos, NewPos))
    {
        m_Visible = false;
        m_pRenderer->setLightColor(vec3(0.0f, 0.0f, 0.0f));
        return;
    }
    
    m_Pos = NewPos;
    m_pRenderer->setLightColor(vec3(800.0f, 600.0f, 400.0f));
    m_pRenderer->setLightPos(m_Pos);
}

void CMissile::render()
{
    if(!m_Visible)
        return; // nothing to do
    
    mat4 PosTrans = translate(mat4(), m_Pos);
    m_pRenderer->setModelTransform(PosTrans);
    m_pRenderer->setMaterial(m_Material);
    m_pMesh->render();
}
