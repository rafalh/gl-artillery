#version 330 core

// Interpolated values from the Vertex Shaders
in vec2 FragmentUV;

// Ouput data
out vec3 PixelColor;

// Values that stay constant for the whole rendering
uniform sampler2D TextureSampler;

void main()
{
	PixelColor = texture(TextureSampler, FragmentUV).rgb;
}
