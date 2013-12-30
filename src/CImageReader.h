#ifndef CIMAGEREADER_H
#define CIMAGEREADER_H

#include "CImage.h"

class CImageReader
{
    public:
        CImage *loadImage(const char *pszPath);
};

#endif // CIMAGEREADER_H
