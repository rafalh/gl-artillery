#version 330 core

// Input vertex data, different for all executions of this shader
layout(location = 0) in vec3 VertexPos;

// Output data - will be interpolated by Fragment Shader
out vec2 FragmentUV;

void main()
{
	gl_Position = vec4(VertexPos, 1.0f);
	FragmentUV = vec2((VertexPos.x + 1.0f)/2.0f, (VertexPos.y + 1.0f)/2.0f);
}
