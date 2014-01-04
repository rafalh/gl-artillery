#include "stdafx.h"
#include "CShield.h"
#include "CRenderer.h"
#include "CGeometryBuilder.h"
#include "CTextureMgr.h"
#include "CShadersMgr.h"
#include "CMesh.h"

using namespace glm;

CShield::CShield(const glm::vec3 &Pos, CRenderer *pRenderer):
    m_Pos(Pos), m_Radius(16.0f),
    m_pRenderer(pRenderer), m_pMesh(nullptr)
{
    m_pMesh = new CMesh;
    CGeometryBuilder Builder;
    Builder.setColor(0x8040A0FF);
    Builder.addIcosphere(2);
    m_pMesh = Builder.createMesh();
    m_Transform = translate(mat4(), Pos) * scale(mat4(), vec3(m_Radius, m_Radius, m_Radius));
}

CShield::~CShield()
{
    delete m_pMesh;
}

void CShield::render()
{
    GLuint OldProgram = m_pRenderer->setProgram(CShadersMgr::getInstance().get("shield"));
    
    m_pRenderer->setModelTransform(m_Transform);
    m_pRenderer->setDoubleSided(true);
    m_pRenderer->setTexture(0);
    m_pRenderer->setProgramUniform("Time", glfwGetTime());
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    m_pMesh->render();
    
    glDisable(GL_BLEND);
    
    m_pRenderer->setDoubleSided(false);
    m_pRenderer->setProgram(OldProgram);
}

bool CShield::testCollision(const glm::vec3 RayBegin, const glm::vec3 RayEnd)
{
    vec3 RayDir = RayEnd - RayBegin;
    vec3 E = RayBegin - m_Pos;
    
    float a = dot(RayDir, RayDir);
    float b = dot(2.0f * E, RayDir);
	float c = dot(E, E) - m_Radius * m_Radius;
	if(b >= 0.0f)
		return false;
	
	float fDelta = b*b - 4.0f*a*c;
	if(fDelta <= 0.0f)
		return false;
	
	float fDeltaSqrt = sqrtf(fDelta);
	float d1 = (-b - fDeltaSqrt) / (2.0f * a);
	float d2 = (-b + fDeltaSqrt) / (2.0f * a);
	if(d1 <= 0.0f || d2 <= 0.0f)
	    return false;
	
	bool bCol = std::min(d1, d2) <= 1.0f;
    return bCol;
}
