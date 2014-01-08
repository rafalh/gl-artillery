#include "stdafx.h"
#include "CImage.h"
#include "CLogger.h"
#include "CException.h"
#include "utils.h"

using namespace std;

CImage::CImage(const char *pszPath)
{
    FREE_IMAGE_FORMAT Format = FreeImage_GetFileType(pszPath, 0);
    if(Format == FIF_UNKNOWN)
        Format = FreeImage_GetFIFFromFilename(pszPath);
    if(Format == FIF_UNKNOWN)
        throw CImageFormatNotRecognizedException("Unknown image format for %s", pszPath);
    
	m_pImg = FreeImage_Load(Format, pszPath);
	if(!m_pImg)
	    throw CImageFormatNotRecognizedException("Failed to load image %s", pszPath);
    
	CLogger::getInstance().info("Loaded %s (fmt %u): type %u bpp %u\n", pszPath, Format,
	    FreeImage_GetImageType(m_pImg), FreeImage_GetBPP(m_pImg));
}

CImage::~CImage()
{
    FreeImage_Unload(m_pImg);
}

RGBQUAD CImage::getPixel(int x, int y)
{
    RGBQUAD *pPal = FreeImage_GetPalette(m_pImg);
    if(pPal)
    {
        BYTE Index;
        if(!FreeImage_GetPixelIndex(m_pImg, x, y, &Index))
            CLogger::getInstance().error("FreeImage_GetPixelIndex failed\n");
        return pPal[Index];
    }
    else
    {
        RGBQUAD Color;
        if(!FreeImage_GetPixelColor(m_pImg, x, y, &Color))
            CLogger::getInstance().error("FreeImage_GetPixelColor failed\n");
        return Color;
    }
}

void CImage::convertToGrayscale16()
{
    if(FreeImage_GetImageType(m_pImg) == FIT_UINT16)
        return; // nothing to do
    
    FIBITMAP *pNewImg = FreeImage_ConvertToUINT16(m_pImg);
    if(!pNewImg)
        throw CInvalidImageException("Failed to convert to grayscale 16-bit");
    
    FreeImage_Unload(m_pImg);
    m_pImg = pNewImg;
}

void *CImage::getScanLine(int y)
{
    return FreeImage_GetScanLine(m_pImg, y);
}

GLuint CImage::createTexture()
{
    CHECK_GL_ERROR();
    
    GLuint Texture = 0;
    glGenTextures(1, &Texture);
    
    glBindTexture(GL_TEXTURE_2D, Texture);
    
    CHECK_GL_ERROR();
    
    unsigned w = FreeImage_GetWidth(m_pImg);
    unsigned h = FreeImage_GetHeight(m_pImg);
    FIBITMAP *pImgCopy = nullptr;
    if(FreeImage_IsTransparent(m_pImg))
    {
        if(FreeImage_GetImageType(m_pImg) != FIT_BITMAP || FreeImage_GetBPP(m_pImg) != 32)
            pImgCopy = FreeImage_ConvertTo32Bits(m_pImg);
        GLvoid *pBits = pImgCopy ? FreeImage_GetBits(pImgCopy) : FreeImage_GetBits(m_pImg);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pBits);
    }
    else
    {
        if(FreeImage_GetImageType(m_pImg) != FIT_BITMAP || FreeImage_GetBPP(m_pImg) != 24)
            pImgCopy = FreeImage_ConvertTo24Bits(m_pImg);
        GLvoid *pBits = pImgCopy ? FreeImage_GetBits(pImgCopy) : FreeImage_GetBits(m_pImg);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pBits);
    }
    
    if(pImgCopy)
        FreeImage_Unload(pImgCopy);
    
    CHECK_GL_ERROR();
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    CHECK_GL_ERROR();
    
	return Texture;
}
