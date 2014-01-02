#include "stdafx.h"
#include "CCannon.h"
#include "CMesh.h"
#include "CRenderer.h"
#include "CGeometryBuilder.h"
#include "CTextureMgr.h"
#include "CLogger.h"

using namespace glm;

CCannon::CCannon(CRenderer *pRenderer):
    m_pRenderer(pRenderer),
    m_pLauncherMesh(nullptr), m_pBaseMesh(nullptr)
{
    prepareBase();
    prepareLauncher();
    
    m_Transform = mat4();
    m_Transform = translate(m_Transform, vec3(50.0f, -6.0f, 50.0f));
    m_Transform = rotate(m_Transform, 225.0f, vec3(0.0f, 1.0f, 0.0f));
}

CCannon::~CCannon()
{
    delete m_pLauncherMesh;
    delete m_pBaseMesh;
}

void CCannon::render()
{
    m_pRenderer->setTexture(CTextureMgr::getInstance().getWhite());
    
    mat4 Rot = rotate(mat4(), 45.0f, vec3(1.0f, 0.0f, 0.0f));
    mat4 LauncherTrans = translate(mat4(), vec3(0.0f, 0.5f, 0.0f));
    m_pRenderer->setModelTransform(m_Transform * LauncherTrans * Rot);
    m_pLauncherMesh->render();
    
    m_pRenderer->setModelTransform(m_Transform);
    m_pBaseMesh->render();
}

void CCannon::prepareBase()
{
    CGeometryBuilder Builder;
    Builder.setColor(0xFF33AACC);
    
    // Front legs
    
    vec3 FrontLeg[] = {
        { 0.5f,  0.0f,  1.0f},
        { 0.5f,  0.0f, -1.0f},
        {-0.5f,  0.0f, -1.0f},
        {-0.5f,  0.0f,  1.0f},
        { 0.5f, -1.0f,  2.0f},
        { 0.5f, -1.0f,  0.0f},
        {-0.5f, -1.0f,  0.0f},
        {-0.5f, -1.0f,  2.0f},
    };
    Builder.setTransform(translate(mat4(), vec3(1.5f, 0.0f, 3.0f)));
    Builder.addBox(FrontLeg);
    
    Builder.setTransform(translate(mat4(), vec3(-1.5f, 0.0f, 3.0f)));
    Builder.addBox(FrontLeg);
    
    // Back legs
    
    vec3 BackLeg[] = {
        { 0.5f,  0.0f,  1.0f},
        { 0.5f,  0.0f, -1.0f},
        {-0.5f,  0.0f, -1.0f},
        {-0.5f,  0.0f,  1.0f},
        { 0.5f, -1.0f,  1.5f},
        { 0.5f, -1.0f, -2.0f},
        {-0.5f, -1.0f, -2.0f},
        {-0.5f, -1.0f,  1.5f},
    };
    Builder.setTransform(translate(mat4(), vec3(1.5f, 0.0f, -3.0f)));
    Builder.addBox(BackLeg);
    
    Builder.setTransform(translate(mat4(), vec3(-1.5f, 0.0f, -3.0f)));
    Builder.addBox(BackLeg);
    
    // Base
    
    mat4 Trans = translate(mat4(), vec3(0.0f, 0.5f, 0.0f));
    mat4 Scale = scale(Trans, vec3(1.8f, 0.5f, 3.5f));
    Builder.setTransform(Scale);
    Builder.addBox();
    
    m_pBaseMesh = Builder.createMesh();
}

void CCannon::prepareLauncher()
{
    CGeometryBuilder Builder;
    Builder.setColor(0xFF33AACC);
    
    mat4 Scale = scale(mat4(), vec3(1.0f, 2.0f, 1.0f));
    mat4 Trans1 = translate(mat4(), vec3(0.0f, 2.0f, 0.0f));
    Builder.setTransform(Trans1 * Scale);
    Builder.addTube(0.35f, 0.6f, 20);
    
    mat4 Trans2 = translate(mat4(), vec3(0.0f, 6.0f, 0.0f));
    Builder.setTransform(Trans2 * Scale);
    Builder.addTube(0.35f, 0.40f, 20);
    
    for(unsigned i = 0; i < 6; ++i)
    {
        mat4 RingTrans = translate(mat4(), vec3(0.0f, 6.0f + i * 0.3f, 0.0f));
        Builder.setTransform(RingTrans);
        Builder.addTorus(0.5f, 0.05f, 20, 20);
    }
    
    m_pLauncherMesh = Builder.createMesh();
}

