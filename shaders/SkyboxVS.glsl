#version 330 core

// Input vertex data, different for all executions of this shader
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexUV;
layout(location = 3) in uint VertexClr;

// Size of texture used for rendering
uniform mat4 MVP;

// Output data - will be interpolated by Fragment Shader
out vec2 FragmentUV;

void main()
{
	vec4 v = vec4(VertexPos, 1);
	gl_Position = MVP * v;
	FragmentUV = VertexUV;
}
