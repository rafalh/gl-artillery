#include "stdafx.h"
#include "CTextureMgr.h"
#include "CImage.h"

GLuint CTextureMgr::get(const std::string &strName)
{
    auto it = m_Map.find(strName);
    if(it != m_Map.end())
        return it->second;
    
    CImage Img(strName.c_str());
    GLuint Texture = Img.createTexture();
    add(strName, Texture);
    return Texture;
}

void CTextureMgr::add(const std::string &strName, GLuint Texture)
{
    m_Map.insert(std::make_pair(strName, Texture));
}

GLuint CTextureMgr::getWhite()
{
    auto it = m_Map.find("white");
    if(it != m_Map.end())
        return it->second;
    
    GLuint Texture = 0;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_BGR, GL_UNSIGNED_BYTE, "\xFF\xFF\xFF");
    add("white", Texture);
    return Texture;
}

void CTextureMgr::clear()
{
    for(auto it = m_Map.begin(); it != m_Map.end(); ++it)
        glDeleteTextures(1, &it->second);
}
