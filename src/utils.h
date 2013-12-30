#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "CLogger.h"

#define CHECK_GL_ERROR() do {\
    GLenum err = glGetError();\
    if(err != GL_NO_ERROR)\
        CLogger::getInstance().error("OpenGL error in %s:%u - 0x%X\n", __FILE__, __LINE__, err);\
    } while(false);

#ifndef FIELD_OFFSET
#  define FIELD_OFFSET(Type, Field) ((uintptr_t)(&(((Type *)(0))->Field)))
#endif

#define ALIGN_UP(Offset, Alignment) (((Offset) + (Alignment)-1) & ~((Alignment)-1))

#define COUNTOF(array) (sizeof(array)/sizeof(array[0]))

#endif // UTILS_H_INCLUDED
