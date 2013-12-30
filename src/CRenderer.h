#ifndef COPENGLDRIVER_H
#define COPENGLDRIVER_H

#include "CCamera.h"
#include "CException.h"

class CInputHandler;

class CRenderer
{
    public:
        CRenderer(unsigned w, unsigned h);
        virtual ~CRenderer();
        void run();
        GLuint loadShaders(const char *vertexShaderPath, const char *fragmentShaderPath);
        void setTitle(const char *pszTitle);
        void setModelTransform(const glm::mat4 &ModelMat, bool bSkybox = false);
        void setTexture(GLuint Texture);

        void setViewMatrix(const glm::mat4 &View)
        {
            m_ViewMatrix = View;
        }
        
        void setProjMatrix(const glm::mat4 &Proj)
        {
            m_ProjMatrix = Proj;
        }

        bool isClosed() const
        {
            return m_bClosed;
        }
        
        void close()
        {
            glfwSetWindowShouldClose(m_pWnd, 1);
        }

        int getWidth() const
        {
            return m_Width;
        }

        int getHeight() const
        {
            return m_Height;
        }

        bool isVisible() const
        {
            return (glfwGetWindowAttrib(m_pWnd, GLFW_VISIBLE) != 0 &&
                glfwGetWindowAttrib(m_pWnd, GLFW_ICONIFIED) == 0);
        }
        
        void setProgram(GLuint Program);
        
        GLuint getProgram()
        {
            return m_Program;
        }
        
        void setProgramUniform(const char *pszName, const glm::vec3 &Value);
        void setInputHandler(CInputHandler *pHandler);
        
        void toggleWireframe()
        {
            m_bWireframe = !m_bWireframe;
        }

    private:
        GLFWwindow *m_pWnd;
        int m_Width, m_Height;
        bool m_bClosed, m_bWireframe;
        
        CInputHandler *m_pInputHandler;
        GLuint m_Program;
        glm::mat4 m_ViewMatrix, m_ProjMatrix;

        static void glfwErrorCallback(int iErrorNum, const char *pszDescription);
        
        //CImage *getCurrentFrame();
        static void onKey(GLFWwindow *pWindow, int Key, int Scancode, int Action, int Mods);
        void updateCamera();
};

DEFINE_EXCEPTION_CLASS(COpenGLException);
DEFINE_EXCEPTION_CLASS(COpenGLShaderException);

#endif // COPENGLDRIVER_H