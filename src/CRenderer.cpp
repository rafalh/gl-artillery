#include "stdafx.h"
#include "CRenderer.h"
#include "CLogger.h"
#include "CException.h"
#include "CInputHandler.h"
#include "CTextureMgr.h"
#include "SMaterial.h"
#include "utils.h"

using namespace std;

void CRenderer::glfwErrorCallback(int iErrorNum, const char *pszDescription)
{
    CLogger::getInstance().error("GLFW error %d: %s\n", iErrorNum, pszDescription);
}

CRenderer::CRenderer(unsigned w, unsigned h):
    m_pWnd(nullptr),
    m_Width(w), m_Height(h),
    m_bClosed(false), m_bWireframe(false),
    m_pInputHandler(nullptr), m_Program(0),
    m_ViewMatrix(glm::mat4(1.0f)), m_ProjMatrix(glm::mat4(1.0f))
{
    // Let user know what happend
    glfwSetErrorCallback(CRenderer::glfwErrorCallback);

    // Initialise GLFW
	if(!glfwInit())
	    throw COpenGLException("Failed to initialize GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	m_pWnd = glfwCreateWindow(m_Width, m_Height, "Artillery Animation", nullptr, nullptr);
	if(!m_pWnd)
	{
		glfwTerminate();
		throw COpenGLException("Failed to create GLFW window");
	}

    glfwMakeContextCurrent(m_pWnd);

    // Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if(glewInit() != GLEW_OK)
		throw COpenGLException("Failed to initialize GLEW");
    
    // Zero error
    glGetError();
    
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    // Use Z-Buffer
    glEnable(GL_DEPTH_TEST);
    
    setDoubleSided(false);
    
	// Set callbacks
	glfwSetWindowUserPointer(m_pWnd, this);
	glfwSetKeyCallback(m_pWnd, onKey);
	glfwSetFramebufferSizeCallback(m_pWnd, onFramebufferSize);
	
	CHECK_GL_ERROR();
}

CRenderer::~CRenderer()
{
    if(m_pWnd)
        glfwDestroyWindow(m_pWnd);

    glfwTerminate();
}

void CRenderer::setTitle(const char *pszTitle)
{
    glfwSetWindowTitle(m_pWnd, pszTitle);
}

GLuint CRenderer::setProgram(GLuint Program)
{
    if(m_Program == Program)
        return m_Program; // nothing to do
    
    GLuint OldProgram = m_Program;
    m_Program = Program;
    glUseProgram(m_Program);
    setModelTransform(glm::mat4(1.0f));
    setProgramUniform("MissilePos", m_LightPos);
    setProgramUniform("MissileColor", m_LightClr);
    CHECK_GL_ERROR();
    
    return OldProgram;
}

void CRenderer::run()
{
    // Check if user closed the window
    if(glfwWindowShouldClose(m_pWnd))
    {
        m_bClosed = true;
        return;
    }

    if(isVisible())
    {
        // Render texture on backbuffer
        glfwMakeContextCurrent(m_pWnd);

        // Swap buffers
        glfwSwapBuffers(m_pWnd);

        // Handle input events if there are any
        glfwPollEvents();

        // Update camera rotation if window has an input focus
        if(glfwGetWindowAttrib(m_pWnd, GLFW_FOCUSED))
            handleCursorMove();
    }
    else
    {
        // Window is hidden so wait for some events - we have nothing to do
        glfwWaitEvents();
    }
    
    if(m_bWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CRenderer::onKey(GLFWwindow *pWindow, int Key, int Scancode, int Action, int Mods)
{
    CRenderer *pThis = static_cast<CRenderer*>(glfwGetWindowUserPointer(pWindow));
    if(pThis->m_pInputHandler)
        pThis->m_pInputHandler->onKey(Key, Action);
}

void CRenderer::onFramebufferSize(GLFWwindow *pWindow, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
    
    CRenderer *pThis = static_cast<CRenderer*>(glfwGetWindowUserPointer(pWindow));
    if(pThis->m_pInputHandler)
        pThis->m_pInputHandler->onFramebufferSize(Width, Height);
}

void CRenderer::setInputHandler(CInputHandler *pHandler)
{
    m_pInputHandler = pHandler;
    
    if(m_pInputHandler)
    {
        // Hide cursor
        glfwSetInputMode(m_pWnd, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Move cursor to center of the window
        double CenterX = m_Width / 2.0, CenterY = m_Height / 2.0;
        glfwSetCursorPos(m_pWnd, CenterX, CenterY);
    }
    else
    {
        // Show cursor
        glfwSetInputMode(m_pWnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void CRenderer::setModelTransform(const glm::mat4 &ModelMat, bool bSkybox)
{
    glm::mat4 MVP;
    if(bSkybox)
    {
        glm::mat4 ViewMat = glm::mat4(glm::mat3(m_ViewMatrix));
        MVP = m_ProjMatrix * ViewMat * ModelMat;
    }
    else
        MVP = m_ProjMatrix * m_ViewMatrix * ModelMat;
    
    GLuint Uniform = glGetUniformLocation(m_Program, "MVP");
    glUniformMatrix4fv(Uniform, 1, GL_FALSE, glm::value_ptr(MVP));
    
    Uniform = glGetUniformLocation(m_Program, "ModelMatrix");
    glUniformMatrix4fv(Uniform, 1, GL_FALSE, glm::value_ptr(ModelMat));
    
    CHECK_GL_ERROR();
}

void CRenderer::setMaterial(const SMaterial &Mat)
{
    setTexture(Mat.Texture);
    setProgramUniform("MaterialAmbientColor", Mat.AmbientColor);
    setProgramUniform("MaterialDiffuseColor", Mat.DiffuseColor);
    setProgramUniform("MaterialSpecularColor", Mat.SpecularColor);
    setProgramUniform("MaterialShininess", Mat.Shininess);
}

void CRenderer::setTexture(GLuint Texture)
{
    // Set texture
    glActiveTexture(GL_TEXTURE0);
    if(!Texture)
        Texture = CTextureMgr::getInstance().getWhite();
    glBindTexture(GL_TEXTURE_2D, Texture);
    GLuint Uniform = glGetUniformLocation(m_Program, "TextureSampler");
    glUniform1i(Uniform, 0);
    CHECK_GL_ERROR();
}

void CRenderer::setProgramUniform(const char *pszName, int Value)
{
    GLuint Uniform = glGetUniformLocation(m_Program, pszName);
    glUniform1i(Uniform, Value);
    CHECK_GL_ERROR();
}

void CRenderer::setProgramUniform(const char *pszName, float Value)
{
    GLuint Uniform = glGetUniformLocation(m_Program, pszName);
    glUniform1f(Uniform, Value);
    CHECK_GL_ERROR();
}

void CRenderer::setProgramUniform(const char *pszName, const glm::vec2 &Value)
{
    GLuint Uniform = glGetUniformLocation(m_Program, pszName);
    glUniform2fv(Uniform, 1, glm::value_ptr(Value));
    CHECK_GL_ERROR();
}

void CRenderer::setProgramUniform(const char *pszName, const glm::vec3 &Value)
{
    GLuint Uniform = glGetUniformLocation(m_Program, pszName);
    glUniform3fv(Uniform, 1, glm::value_ptr(Value));
    CHECK_GL_ERROR();
}

void CRenderer::setProgramUniform(const char *pszName, const glm::vec4 &Value)
{
    GLuint Uniform = glGetUniformLocation(m_Program, pszName);
    glUniform4fv(Uniform, 1, glm::value_ptr(Value));
    CHECK_GL_ERROR();
}

void CRenderer::setDoubleSided(bool DoubleSided)
{
    if(DoubleSided)
        glDisable(GL_CULL_FACE);
    else
        glEnable(GL_CULL_FACE);
    CHECK_GL_ERROR();
}

void CRenderer::clear(const glm::vec3 &Color)
{
    // Clear the screen
    glClearColor(Color.x, Color.y, Color.z, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    CHECK_GL_ERROR();
}

void CRenderer::handleCursorMove()
{
    // Get cursor offset from center of the window
    double CursorX, CursorY;
    double CenterX = m_Width / 2.0, CenterY = m_Height / 2.0;
    glfwGetCursorPos(m_pWnd, &CursorX, &CursorY);
    float fDeltaX = (float)((CursorX - CenterX) / CenterX);
    float fDeltaY = (float)(-(CursorY - CenterY) / CenterY);
    glfwSetCursorPos(m_pWnd, CenterX, CenterY);
    
    if(m_pInputHandler)
        m_pInputHandler->onMouseMove(fDeltaX, fDeltaY);
}
