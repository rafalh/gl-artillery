#include "stdafx.h"
#include "CMesh.h"
#include "utils.h"

CMesh::CMesh():
    m_VertexArray(0),
    m_VertexBuf(0), m_ElementBuf(0)
{
    // Create Vertex Array Object
    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);
    
    CHECK_GL_ERROR();
}

CMesh::~CMesh()
{
    if(m_VertexBuf)
        glDeleteBuffers(1, &m_VertexBuf);
    
    if(m_ElementBuf)
        glDeleteBuffers(1, &m_ElementBuf);
    
    if(m_VertexArray)
        glDeleteVertexArrays(1, &m_VertexArray);
    
    CHECK_GL_ERROR();
}

void CMesh::setVertices(const SVertex *pVertices, unsigned Count)
{
    glBindVertexArray(m_VertexArray);
    glGenBuffers(1, &m_VertexBuf);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SVertex) * Count, pVertices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();
	
	// Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                   // attribute 0
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        sizeof(SVertex), // stride
        (GLvoid*)FIELD_OFFSET(SVertex, Pos) // array buffer offset
    );
    
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                   // attribute 0
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        sizeof(SVertex),     // stride
        (GLvoid*)FIELD_OFFSET(SVertex, Normal) // array buffer offset
    );
    
    // UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                   // attribute 2
        2,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        sizeof(SVertex),     // stride
        (GLvoid*)FIELD_OFFSET(SVertex, UV) // array buffer offset
    );
    
    // Color
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(
        3,                   // attribute 3
        1,                   // size
        GL_UNSIGNED_INT,     // type
        sizeof(SVertex),     // stride
        (GLvoid*)FIELD_OFFSET(SVertex, Clr) // array buffer offset
    );
    
    CHECK_GL_ERROR();
}

void CMesh::setIndices(const uint32_t *pIndices, unsigned Count)
{
    m_IndicesCount = Count;
    glBindVertexArray(m_VertexArray);
    glGenBuffers(1, &m_ElementBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*pIndices) * m_IndicesCount, pIndices, GL_STATIC_DRAW);
    m_IndexType = GL_UNSIGNED_INT;
    CHECK_GL_ERROR();
}

void CMesh::setIndices(const uint16_t *pIndices, unsigned Count)
{
    m_IndicesCount = Count;
    glBindVertexArray(m_VertexArray);
    glGenBuffers(1, &m_ElementBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*pIndices) * m_IndicesCount, pIndices, GL_STATIC_DRAW);
    m_IndexType = GL_UNSIGNED_SHORT;
    CHECK_GL_ERROR();
}

void CMesh::render()
{
    // Draw triangles
    glBindVertexArray(m_VertexArray);
	glDrawElements(GL_TRIANGLES, m_IndicesCount, m_IndexType, nullptr);
	CHECK_GL_ERROR();
}

void CMesh::renderPart(unsigned Offset, unsigned Count)
{
    // Draw triangles
    Offset *= m_IndexType == GL_UNSIGNED_SHORT ? 2 : 4;
    glBindVertexArray(m_VertexArray);
	glDrawElements(GL_TRIANGLES, Count, m_IndexType, (GLvoid*)Offset);
	CHECK_GL_ERROR();
}
