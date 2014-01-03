#include "CSkybox.h"
#include "CTextureMgr.h"
#include "CShadersMgr.h"
#include "CRenderer.h"
#include "utils.h"

CSkybox::CSkybox(const char *pszFilenamePattern, CRenderer *pRenderer):
    m_pRenderer(pRenderer)
{
    char Buf[256];
    sprintf(Buf, pszFilenamePattern, "up");
    m_Textures[0] = CTextureMgr::getInstance().get(Buf);
    sprintf(Buf, pszFilenamePattern, "north");
    m_Textures[1] = CTextureMgr::getInstance().get(Buf);
    sprintf(Buf, pszFilenamePattern, "west");
    m_Textures[2] = CTextureMgr::getInstance().get(Buf);
    sprintf(Buf, pszFilenamePattern, "south");
    m_Textures[3] = CTextureMgr::getInstance().get(Buf);
    sprintf(Buf, pszFilenamePattern, "east");
    m_Textures[4] = CTextureMgr::getInstance().get(Buf);
    sprintf(Buf, pszFilenamePattern, "down");
    m_Textures[5] = CTextureMgr::getInstance().get(Buf);
    
    SVertex Vertices[] = {
        // Up
        {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        
        // North
        {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        
        // West
        {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        
        // South
        {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        
        // East
        {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        
        // Down
        {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
    };
    
    uint16_t Indices[] = {
        // Up
        0, 1, 2,
        0, 2, 3,
        // North
        4, 5, 6,
        4, 6, 7,
        // West
        8, 9, 10,
        8, 10, 11,
        // South
        12, 13, 14,
        12, 14, 15,
        // East
        16, 17, 18,
        16, 18, 19,
        // Down
        20, 21, 22,
        20, 22, 23,
    };
    
    m_pMesh = new CMesh;
    m_pMesh->setVertices(Vertices, COUNTOF(Vertices));
    m_pMesh->setIndices(Indices, COUNTOF(Indices));
}

CSkybox::~CSkybox()
{
    delete m_pMesh;
}

void CSkybox::render()
{
    GLuint OldProgram = m_pRenderer->setProgram(CShadersMgr::getInstance().get("skybox"));
    m_pRenderer->setModelTransform(glm::mat4(1.0f), true);
    
    for(unsigned i = 0; i < 6; ++i)
    {
        m_pRenderer->setTexture(m_Textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        m_pMesh->render(CMesh::SOffsetSize(i * 6, 6));
    }
    
    m_pRenderer->setProgram(OldProgram);
}
