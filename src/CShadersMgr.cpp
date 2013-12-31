#include "stdafx.h"
#include "CShadersMgr.h"
#include "CException.h"
#include "CLogger.h"

using namespace std;

CShadersMgr::CShadersMgr()
{}

void CShadersMgr::clear()
{
    for(auto it = m_Map.begin(); it != m_Map.end(); ++it)
        glDeleteProgram(it->second);
}

GLuint CShadersMgr::get(const std::string &Name)
{
    auto it = m_Map.find(Name);
    if(it != m_Map.end())
        return it->second;
    
    char vs[256], fs[256];
    sprintf(vs, "shaders/%.128s.vs.glsl", Name.c_str());
    sprintf(fs, "shaders/%.128s.fs.glsl", Name.c_str());
    GLuint ShaderProgram = load(vs, fs);
    m_Map.insert(make_pair(Name, ShaderProgram));
    
    return ShaderProgram;
}

GLuint CShadersMgr::load(const char *VertexShaderPath, const char *FragmentShaderPath)
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
