#version 330 core

// Interpolated values from the Vertex Shaders
in vec2 FragmentUV;
in vec4 FragmentColor;

// Ouput data
out vec4 PixelColor;

// Values that stay constant for the whole rendering
uniform sampler2D TextureSampler;

void main()
{
	PixelColor = texture(TextureSampler, FragmentUV) * FragmentColor;
}
