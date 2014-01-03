#ifndef CIMAGE_H
#define CIMAGE_H

#include "CException.h"

class CImage
{
    public:
        CImage(const char *pszPath);
        ~CImage();
        GLuint createTexture();
        
        RGBQUAD getPixel(int x, int y);
        void convertToGrayscale16();
        void *getScanLine(int y);
        
        int getWidth() const
        {
            return FreeImage_GetWidth(m_pImg);
        }
        
        int getHeight() const
        {
            return FreeImage_GetHeight(m_pImg);
        }
    
    private:
        FIBITMAP *m_pImg;
};

DEFINE_EXCEPTION_CLASS(CInvalidImageException);
DEFINE_EXCEPTION_CLASS(CImageFormatNotRecognizedException);

#endif // CIMAGE_H
