#version 330 core

// Interpolated values from the Vertex Shaders
in vec2 FragmentUV;

// Ouput data
out vec3 PixelColor;

// Values that stay constant for the whole rendering
uniform sampler2D TextureSampler;
uniform vec2 TexelSize;
uniform int IsVertical;

float Gaussian(float x, float deviation)
{
	return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));	
}

void main()
{
	// Settings
	const int BlurAmount = 16;
	const float BlurStrength = 0.3f;
	const float BlurScale = 1.0f;
	
	// Locals
	float HalfBlur = float(BlurAmount) * 0.5;
	vec3 Color = vec3(0.0f, 0.0f, 0.0f);
	
	// Gaussian deviation
	float Deviation = HalfBlur * 0.35;
	Deviation *= Deviation;
	float Strength = 1.0 - BlurStrength;
	
	if(IsVertical == 1)
	{
		// Vertical blur
		for(int i = 0; i < BlurAmount; ++i)
		{
			float Offset = float(i) - HalfBlur;
			Color += texture(TextureSampler, FragmentUV + vec2(0.0, Offset * TexelSize.y * BlurScale)).rgb * Gaussian(Offset * Strength, Deviation);
		}
	}
	else
	{
		// Horizontal blur
		for(int i = 0; i < BlurAmount; ++i)
		{
			float Offset = float(i) - HalfBlur;
			Color += texture(TextureSampler, FragmentUV + vec2(Offset * TexelSize.x * BlurScale, 0.0)).rgb * Gaussian(Offset * Strength, Deviation);
		}
	}
	
	PixelColor = Color;
}
