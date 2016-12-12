#include "stdafx.h"
#include "CCannon.h"
#include "CMesh.h"
#include "CRenderer.h"
#include "CSceneMgr.h"
#include "CGeometryBuilder.h"
#include "CTextureMgr.h"
#include "SMaterial.h"
#include "CLogger.h"
#include "CMissile.h"
#include "utils.h"

using namespace glm;

static const float PI = 3.141592f;

CCannon::CCannon(const glm::vec3 &Pos, CRenderer *pRenderer, CSceneMgr *pSceneMgr):
    m_pRenderer(pRenderer), m_pSceneMgr(pSceneMgr),
    m_pMesh(nullptr), m_pMissile(nullptr),
    m_AngleX(0), m_AngleY(0),
    m_MissileVisible(false)
{
    m_CannonMaterial.AmbientColor = vec3(0.1f, 0.1f, 0.1f);
    m_CannonMaterial.DiffuseColor = vec3(1.0f, 1.0f, 1.0f);
    m_CannonMaterial.SpecularColor = vec3(0.3f, 0.3f, 0.3f);
    m_CannonMaterial.Shininess = 64.0f;
    m_CannonMaterial.Texture = CTextureMgr::getInstance().get("textures/cannon.jpg");
    
    m_DisabledRingMaterial.AmbientColor = vec3(0.2f, 0.0f, 0.0f);
    m_DisabledRingMaterial.DiffuseColor = vec3(0.3f, 0.0f, 0.0f);
    
    m_EnabledRingMaterial.AmbientColor = vec3(0.6f, 0.0f, 0.0f);
    m_EnabledRingMaterial.DiffuseColor = vec3(0.4f, 0.0f, 0.0f);
    
    CGeometryBuilder Builder;
    Builder.setColor(0xFF337799);
    
    prepareBase(Builder);
    prepareLauncher(Builder);
    m_pMesh = Builder.createMesh();
    
    m_Transform = translate(mat4(), Pos) * rotate(m_Transform, -45.0f, vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
    
    m_PrevFrameTime = glfwGetTime();
    m_ShotTime = m_PrevFrameTime - SHOT_TIME;
}

CCannon::~CCannon()
{
    delete m_pMesh;
}

void CCannon::animate()
{
    const float ROT_X_SPEED = 5.0f;
    const float ROT_Y_SPEED = 5.0f;
    
    float dt = glfwGetTime() - m_PrevFrameTime;
    float SignX = m_DestAngleX > m_AngleX ? 1.0f : -1.0f;
    float SignY = m_DestAngleY > m_AngleY ? 1.0f : -1.0f;
    m_AngleX = m_AngleX + SignX * std::min(fabsf(m_DestAngleX - m_AngleX), ROT_X_SPEED * dt);
    m_AngleY = m_AngleY + SignY * std::min(fabsf(m_DestAngleY - m_AngleY), ROT_Y_SPEED * dt);
    
    m_PrevFrameTime = glfwGetTime();
}

void CCannon::render()
{
    m_pRenderer->setMaterial(m_CannonMaterial);
    
    float ShotDelta = glfwGetTime() - m_ShotTime;
    float ShotProgress = ShotDelta > SHOT_TIME ? 0.0f : (ShotDelta / SHOT_TIME);
    
    mat4 BaseTransform = m_Transform * translate(mat4(), vec3(0.0f, sinf(glfwGetTime() * 3.0f) * 0.1f, 0.0f));
    mat4 RotatorTransform = translate(mat4(), vec3(0.0f, 0.9f, 0.9f));
    mat4 FrontGunTransform = translate(mat4(), vec3(0.0f, 2.0f, -0.6f + 1.5f));
    mat4 BackGunTransform = translate(mat4(), vec3(0.0f, 2.0f, -0.6f + 1.5f));
    
    if(ShotProgress < 0.15f) // 0.00 - 0.15 ms
        FrontGunTransform = FrontGunTransform * translate(mat4(), vec3(0.0f, 0.0f, -sinf(ShotProgress/0.15f * PI/2.0f)));
    else if(ShotProgress < 0.25f) // 0.15 - 0.25 ms
        FrontGunTransform = FrontGunTransform * translate(mat4(), vec3(0.0f, 0.0f, -1.0f));
    else // 0.25 - 1.00 ms
        FrontGunTransform = FrontGunTransform * translate(mat4(), vec3(0.0f, 0.0f, -1.0f + (ShotProgress-0.25f)/0.75f));
    
    if(ShotProgress < 0.10f) // 0.00 - 0.10 ms
        ; //nothing
    else if(ShotProgress < 0.25f) // 0.10 - 0.25 ms
        BackGunTransform = BackGunTransform * translate(mat4(), vec3(0.0f, 0.0f, -0.5f * sinf((ShotProgress-0.10f)/0.15f * PI/2.0f)));
    else // 0.25 - 1.00 ms
        BackGunTransform = BackGunTransform * translate(mat4(), vec3(0.0f, 0.0f, -0.5f + 0.5f * (ShotProgress-0.25f)/0.75f));
    
    mat4 RotationX = rotate(mat4(), m_AngleX, vec3(-1.0f, 0.0f, 0.0f));
    RotationX = translate(mat4(), vec3(0.0f, 0.2f, -0.1f)) * RotationX * translate(mat4(), vec3(0.0f, -0.2f, 0.1f));
    
    vec3 RotAxisY(RotationX * vec4(0.0f, -1.0f, 0.0f, 0.0f));
    vec3 RotPosY(RotationX * vec4(0.0f, 0.0f, -0.6f + 1.5f, 1.0f));
    mat4 RotationY = rotate(mat4(), m_AngleY, RotAxisY);
    RotationY = translate(mat4(), RotPosY) * RotationY * translate(mat4(), -RotPosY);
    
    m_BarrelTransform = BaseTransform * RotationY * RotationX * FrontGunTransform;
    m_pRenderer->setModelTransform(m_BarrelTransform);
    m_pMesh->render(m_FrontGun);
    
    unsigned ShiningRings = std::min((unsigned)(ShotDelta * 2.0f), m_BarrelRings.size());
    if(ShiningRings > 0)
        m_pRenderer->setMaterial(m_EnabledRingMaterial);
    for(unsigned i = 0; i < ShiningRings; ++i)
        m_pMesh->render(m_BarrelRings[i]);
    if(ShiningRings < m_BarrelRings.size())
        m_pRenderer->setMaterial(m_DisabledRingMaterial);
    for(unsigned i = ShiningRings; i < m_BarrelRings.size(); ++i)
        m_pMesh->render(m_BarrelRings[i]);
    
    m_pRenderer->setMaterial(m_CannonMaterial);
    m_pRenderer->setModelTransform(BaseTransform * RotationY * RotationX * BackGunTransform);
    m_pMesh->render(m_BackGun);
    
    m_pRenderer->setModelTransform(BaseTransform * RotationY * RotationX * RotatorTransform);
    m_pMesh->render(m_RotatorY);
    
    m_pRenderer->setModelTransform(BaseTransform * RotationX);
    m_pMesh->render(m_BarrelBase);
    m_pMesh->render(m_RotatorX);
    
    m_pRenderer->setModelTransform(BaseTransform);
    m_pMesh->render(m_CannonBase);
}

void CCannon::prepareBase(CGeometryBuilder &Builder)
{
    CGeometryBuilder BaseSideBuilder;
    BaseSideBuilder.setColor(Builder.getColor());
    
    // Front hover
    vec3 FrontHover[] = {
        { 0.6f,  0.0f,  1.5f},
        { 0.6f,  0.0f, -1.0f},
        {-0.6f,  0.0f, -1.0f},
        {-0.6f,  0.0f,  1.5f},
        { 1.2f, -1.0f,  2.5f},
        { 1.2f, -1.0f,  0.0f},
        {-0.8f, -1.0f,  0.0f},
        {-0.8f, -1.0f,  2.5f},
    };
    BaseSideBuilder.setTransform(translate(mat4(), vec3(1.5f, 0.0f, 3.0f)));
    BaseSideBuilder.addBox(FrontHover);
    
    // Back hover
    vec3 BackHover[] = {
        { 0.6f,  0.0f,  1.0f},
        { 0.6f,  0.0f, -1.0f},
        {-0.6f,  0.0f, -1.0f},
        {-0.6f,  0.0f,  1.0f},
        { 1.2f, -1.0f,  1.5f},
        { 1.2f, -1.0f, -2.0f},
        {-0.8f, -1.0f, -2.0f},
        {-0.8f, -1.0f,  1.5f},
    };
    BaseSideBuilder.setTransform(translate(mat4(), vec3(1.5f, 0.0f, -3.0f)));
    BaseSideBuilder.addBox(BackHover);
    
    // Base side - right face
    vec3 SideBasePos1[8] = {
        { 0.5f,  1.2f, -2.9f},
        { 0.5f,  1.2f, -1.4f},
        { 0.5f,  0.0f, -0.8f},
        { 0.5f,  0.0f,  1.7f},
        { 0.5f, -0.8f,  2.4f},
        { 0.5f, -0.8f, -1.2f},
        { 0.5f,  0.3f, -1.8f},
        { 0.5f,  0.3f, -3.5f},
    };
    std::uint16_t SideBaseIdx1[] = {
        0, 6, 7,
        0, 1, 6,
        1, 5, 6,
        1, 2, 5,
        2, 3, 5,
        3, 4, 5,
    };
    
    // Base side - left face
    vec3 SideBasePos2[COUNTOF(SideBasePos1)];
	std::uint16_t SideBaseIdx2[COUNTOF(SideBaseIdx1)];
    memcpy(SideBasePos2, SideBasePos1, sizeof(SideBasePos1));
    memcpy(SideBaseIdx2, SideBaseIdx1, sizeof(SideBaseIdx1));
    
    for(unsigned i = 0; i < COUNTOF(SideBasePos1); ++i)
        SideBasePos2[i].x = -SideBasePos2[i].x;
    for(unsigned i = 0; i < COUNTOF(SideBaseIdx1); i += 3)
        std::swap(SideBaseIdx2[i], SideBaseIdx2[i + 1]);
    
    // Base side
    BaseSideBuilder.setTransform(translate(mat4(), vec3(1.25f, 0.0f, 0.0f)) * scale(mat4(), vec3(0.5f, 1.0f, 1.0f)));
    BaseSideBuilder.addPolygon(SideBasePos1, COUNTOF(SideBasePos1), SideBaseIdx1, COUNTOF(SideBaseIdx1));
    BaseSideBuilder.addPolygon(SideBasePos2, COUNTOF(SideBasePos2), SideBaseIdx2, COUNTOF(SideBaseIdx2));
    BaseSideBuilder.addQuad(SideBasePos2[0], SideBasePos2[1], SideBasePos1[1], SideBasePos1[0]);
    BaseSideBuilder.addQuad(SideBasePos2[1], SideBasePos2[2], SideBasePos1[2], SideBasePos1[1]);
    BaseSideBuilder.addQuad(SideBasePos2[2], SideBasePos2[3], SideBasePos1[3], SideBasePos1[2]);
    BaseSideBuilder.addQuad(SideBasePos2[3], SideBasePos2[4], SideBasePos1[4], SideBasePos1[3]);
    BaseSideBuilder.addQuad(SideBasePos2[4], SideBasePos2[5], SideBasePos1[5], SideBasePos1[4]);
    BaseSideBuilder.addQuad(SideBasePos2[5], SideBasePos2[6], SideBasePos1[6], SideBasePos1[5]);
    BaseSideBuilder.addQuad(SideBasePos2[6], SideBasePos2[7], SideBasePos1[7], SideBasePos1[6]);
    BaseSideBuilder.addQuad(SideBasePos2[7], SideBasePos2[0], SideBasePos1[0], SideBasePos1[7]);
    
    // Hover connector
    vec3 HoverConnector[] = {
        { 0.5f,  0.3f, -2.1f},
        { 0.5f,  0.3f, -3.3f},
        {-0.5f,  0.3f, -3.3f},
        {-0.5f,  0.3f, -2.1f},
        { 0.5f,  0.0f, -2.3f},
        { 0.5f,  0.0f, -3.5f},
        {-0.5f,  0.0f, -3.5f},
        {-0.5f,  0.0f, -2.3f},
    };
    BaseSideBuilder.setTransform(translate(mat4(), vec3(1.25f, 0.0f, 0.0f)) * scale(mat4(), vec3(0.4f, 1.0f, 1.0f)));
    BaseSideBuilder.addBox(HoverConnector);
    
    // Add right and left side to main builder
    Builder.setTransform(mat4());
    Builder.addVertices(BaseSideBuilder.getVertices(), BaseSideBuilder.getIndices());
    
    Builder.setTransform(scale(mat4(), vec3(-1.0f, 1.0f, 1.0f)));
    BaseSideBuilder.switchVerticesOrder();
    Builder.addVertices(BaseSideBuilder.getVertices(), BaseSideBuilder.getIndices());
    
    // Center axle
    vec3 CenterAxle[] = {
        { 0.6f,  0.0f,  4.0f},
        { 0.6f,  0.0f, -2.0f},
        {-0.6f,  0.0f, -2.0f},
        {-0.6f,  0.0f,  4.0f},
        { 0.6f, -1.0f,  3.0f},
        { 0.6f, -1.0f, -3.0f},
        {-0.6f, -1.0f, -3.0f},
        {-0.6f, -1.0f,  3.0f},
    };
    Builder.setTransform(mat4());
    Builder.addBox(CenterAxle);
    
    // Center axle - side axles connector
    vec3 CenterSideCon[] = {
        { 1.0f, -0.3f,  2.8f},
        { 1.0f, -0.3f, -1.0f},
        {-1.0f, -0.3f, -1.0f},
        {-1.0f, -0.3f,  2.8f},
        { 1.0f, -0.7f,  2.8f},
        { 1.0f, -0.7f, -1.0f},
        {-1.0f, -0.7f, -1.0f},
        {-1.0f, -0.7f,  2.8f},
    };
    Builder.setTransform(mat4());
    Builder.addBox(CenterSideCon);
    m_CannonBase = Builder.subMesh();
    
    // Launcher base - center axle connector
    vec3 LauncherAxleCon[] = {
        { 0.4f,  0.2f,  2.5f},
        { 0.4f,  0.2f, -0.0f},
        {-0.4f,  0.2f, -0.0f},
        {-0.4f,  0.2f,  2.5f},
        { 0.4f, -1.0f,  2.5f},
        { 0.4f, -0.2f, -0.0f},
        {-0.4f, -0.2f, -0.0f},
        {-0.4f, -1.0f,  2.5f},
    };
    Builder.setTransform(mat4());
    Builder.addBox(LauncherAxleCon);
    m_RotatorX = Builder.subMesh();
    
    // Launcher Base
    vec3 LauncherBase[] = {
        { 1.2f,  0.8f,  2.5f},
        { 1.2f,  0.8f, -0.5f},
        {-1.2f,  0.8f, -0.5f},
        {-1.2f,  0.8f,  2.5f},
        { 1.8f,  0.2f,  3.5f},
        { 1.8f,  0.2f, -0.1f},
        {-1.8f,  0.2f, -0.1f},
        {-1.8f,  0.2f,  3.5f},
    };
    Builder.setTransform(mat4());
    Builder.addBox(LauncherBase);
    m_BarrelBase = Builder.subMesh();
    
    // Launcher rotator
    vec3 LauncherRotator[] = {
        { 1.0f,  0.1f,  1.1f},
        { 1.0f,  0.1f, -1.1f},
        {-1.0f,  0.1f, -1.1f},
        {-1.0f,  0.1f,  1.1f},
        { 1.0f, -0.1f,  1.1f},
        { 1.0f, -0.1f, -1.1f},
        {-1.0f, -0.1f, -1.1f},
        {-1.0f, -0.1f,  1.1f},
    };
    Builder.setTransform(mat4());
    Builder.addBox(LauncherRotator);
    m_RotatorY = Builder.subMesh();
}

void CCannon::prepareLauncher(CGeometryBuilder &Builder)
{
    mat4 Rot = rotate(mat4(), 90.0f, vec3(1.0f, 0.0f, 0.0f));
    
    // Smaller tube
    mat4 Trans2 = translate(mat4(), vec3(0.0f, 0.0f, 6.0f));
    mat4 Scale2 = scale(mat4(), vec3(1.0f, 1.0f, 3.0f));
    Builder.setTransform(Trans2 * Scale2 * Rot);
    Builder.addTube(0.35f, 0.50f, 20);
    
    m_FrontGun = Builder.subMesh();
    
    // Rings
	std::uint32_t OldColor = Builder.setColor(0xFFFFFFFF);
    for(unsigned i = 0; i < 6; ++i)
    {
        mat4 RingTrans = translate(mat4(), vec3(0.0f, 0.0f, 5.5f + i * 0.3f));
        Builder.setTransform(RingTrans * Rot);
        Builder.addTorus(0.6f, 0.05f, 20, 20);
        m_BarrelRings.push_back(Builder.subMesh());
    }
    Builder.setColor(OldColor);
    
    // Larger tube
    mat4 Scale1 = scale(mat4(), vec3(1.0f, 1.0f, 2.0f));
    mat4 Trans1 = translate(mat4(), vec3(0.0f, 0.0f, 1.0f));
    Builder.setTransform(Trans1 * Scale1 * Rot);
    Builder.addTube(0.35f, 0.65f, 20);
    
    // Launcher fragment
    CGeometryBuilder LauncherPartBuilder;
    LauncherPartBuilder.setColor(Builder.getColor());
    vec3 Launcher[] = {
        { 0.9f,  1.0f,  1.4f},
        { 0.9f,  1.0f,  0.3f},
        { 0.1f,  1.0f,  0.3f},
        { 0.1f,  1.0f,  1.4f},
        { 1.0f,  0.0f,  1.5f},
        { 1.0f,  0.0f,  0.3f},
        { 0.0f,  0.0f,  0.3f},
        { 0.0f,  0.0f,  1.5f},
    };
    LauncherPartBuilder.setTransform(translate(mat4(), vec3(0.0f, 0.0f, 0.0f)));
    LauncherPartBuilder.addBox(Launcher);
    
    // Launcher half
    CGeometryBuilder LauncherHalfBuilder;
    LauncherHalfBuilder.setColor(Builder.getColor());
    for(unsigned i = 0; i < 4; ++i)
    {
        LauncherHalfBuilder.setTransform(scale(mat4(), vec3((i % 2) ? -1.0f : 1.0f, (i < 2) ? 1.0f : -1.0f, 1.0f)));
        LauncherHalfBuilder.addVertices(LauncherPartBuilder.getVertices(), LauncherPartBuilder.getIndices());
        if(i == 0 || i == 2)
            LauncherPartBuilder.switchVerticesOrder();
    }
    
    // Entire launcher
    Builder.setTransform(mat4());
    Builder.addVertices(LauncherHalfBuilder.getVertices(), LauncherHalfBuilder.getIndices());
    Builder.setTransform(scale(mat4(), vec3(1.0f, 1.0f, -1.0f)));
    LauncherHalfBuilder.switchVerticesOrder();
    Builder.addVertices(LauncherHalfBuilder.getVertices(), LauncherHalfBuilder.getIndices());
    
    m_BackGun = Builder.subMesh();
}

void CCannon::fire()
{ 
    if(!m_pMissile)
    {
        m_pMissile = new CMissile(m_pRenderer, m_pSceneMgr);
        m_pSceneMgr->add(m_pMissile);
    }
    
    vec3 StartPos(m_BarrelTransform * vec4(0.0f, 0.0f, 6.0f, 1.0f));
    vec3 Velocity(mat3(m_BarrelTransform) * vec3(0.0f, 0.0f, 88.0f));
    m_pMissile->setVelocity(Velocity);
    m_pMissile->start(StartPos);
    
    m_ShotTime = glfwGetTime();
}
