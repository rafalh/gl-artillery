#include "stdafx.h"
#include "CRenderer.h"
#include "CLogger.h"
#include "CException.h"
#include "CInputHandler.h"
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
    
    glEnable(GL_CULL_FACE);

	// Set callbacks
	glfwSetWindowUserPointer(m_pWnd, this);
	glfwSetKeyCallback(m_pWnd, onKey);
	
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

void CRenderer::setProgram(GLuint Program)
{
    m_Program = Program;
    glUseProgram(m_Program);
    setModelTransform(glm::mat4(1.0f));
    CHECK_GL_ERROR();
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
            updateCamera();
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

GLuint CRenderer::loadShaders(const char *VertexShaderPath, const char *FragmentShaderPath)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	string VertexShaderCode;
    ifstream VertexShaderStream(VertexShaderPath, ios::in);
	if(VertexShaderStream.is_open())
	{
		string Line;
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		glDeleteShader(VertexShaderID);
	    glDeleteShader(FragmentShaderID);
	    throw CFileNotFoundException("Failed to open %s", VertexShaderPath);
	}

	// Read the Fragment Shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(FragmentShaderPath, ios::in);
	if(FragmentShaderStream.is_open())
	{
		string Line;
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else
	{
		glDeleteShader(VertexShaderID);
	    glDeleteShader(FragmentShaderID);
	    throw CFileNotFoundException("Failed to open %s", FragmentShaderPath);
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	CLogger::getInstance().info("Compiling shader: %s\n", VertexShaderPath);
	char const *VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(InfoLogLength > 0)
	{
		unique_ptr<char[]> InfoBuf(new char[InfoLogLength + 1]);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, InfoBuf.get());
		if(InfoBuf[0])
		    CLogger::getInstance().info("%s", InfoBuf.get());
	}
	if(!Result)
	{
	    glDeleteShader(VertexShaderID);
	    glDeleteShader(FragmentShaderID);
	    throw COpenGLShaderException("Failed to compile %s", VertexShaderPath);
	}

	// Compile Fragment Shader
	CLogger::getInstance().info("Compiling shader: %s\n", FragmentShaderPath);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(InfoLogLength > 0)
	{
		unique_ptr<char[]> InfoBuf(new char[InfoLogLength + 1]);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, InfoBuf.get());
		if(InfoBuf[0])
		    CLogger::getInstance().info("%s", InfoBuf.get());
	}
	if(!Result)
	{
	    glDeleteShader(VertexShaderID);
	    glDeleteShader(FragmentShaderID);
	    throw COpenGLShaderException("Failed to compile %s", FragmentShaderPath);
	}

	// Link the program
	CLogger::getInstance().info("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

    // Shaders are not needed anymore
    glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(InfoLogLength > 0)
	{
		unique_ptr<char[]> InfoBuf(new char[InfoLogLength + 1]);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, InfoBuf.get());
		if(InfoBuf[0])
		    CLogger::getInstance().info("%s", InfoBuf.get());
	}
	if(!Result)
	{
	    glDeleteProgram(ProgramID);
		throw COpenGLShaderException("Failed to link OpenGL program");
	}

	return ProgramID;
}

void CRenderer::onKey(GLFWwindow *pWindow, int Key, int Scancode, int Action, int Mods)
{
    CRenderer *pThis = static_cast<CRenderer*>(glfwGetWindowUserPointer(pWindow));
    if(pThis->m_pInputHandler)
        pThis->m_pInputHandler->onKey(Key, Action);
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
        glm::mat4 ViewMat = m_ViewMatrix;
        ViewMat[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        MVP = m_ProjMatrix * ViewMat * ModelMat;
    }
    else
        MVP = m_ProjMatrix * m_ViewMatrix * ModelMat;
    
    GLuint Uniform = glGetUniformLocation(m_Program, "MVP");
    glUniformMatrix4fv(Uniform, 1, GL_FALSE, glm::value_ptr(MVP));
    CHECK_GL_ERROR();
}

void CRenderer::setTexture(GLuint Texture)
{
    // Set texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    GLuint Uniform = glGetUniformLocation(m_Program, "TextureSampler");
    glUniform1i(Uniform, 0);
    CHECK_GL_ERROR();
}

void CRenderer::setProgramUniform(const char *pszName, const glm::vec3 &Value)
{
    GLuint Uniform = glGetUniformLocation(m_Program, pszName);
    glUniform3fv(Uniform, 1, glm::value_ptr(Value));
    CHECK_GL_ERROR();
}

void CRenderer::updateCamera()
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
