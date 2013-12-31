#ifndef CSHADERSMGR_H
#define CSHADERSMGR_H

#include "CException.h"

class CShadersMgr
{
    public:
        static CShadersMgr &getInstance()
        {
            static CShadersMgr Instance;
            return Instance;
        }
        
        void clear();
        GLuint get(const std::string &Name);
        
    private:
        std::unordered_map<std::string, GLuint> m_Map;
        
        CShadersMgr();
        GLuint load(const char *VertexShaderPath, const char *FragmentShaderPath);
        
        ~CShadersMgr()
        {
            clear();
        }
};

DEFINE_EXCEPTION_CLASS(COpenGLShaderException);

#endif // CSHADERSMGR_H
