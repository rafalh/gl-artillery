#version 330 core

// Interpolated values from the Vertex Shaders
in vec3 FragmentPos;
in vec3 FragmentNormal;
in vec2 FragmentUV;
in vec4 FragmentColor;

// Ouput data
out vec4 PixelColor;

// Values that stay constant for the whole rendering
uniform sampler2D TextureSampler;
uniform vec3 EyePos;
uniform vec3 MaterialAmbientColor;
uniform vec3 MaterialDiffuseColor;
uniform vec3 MaterialSpecularColor;
uniform float MaterialShininess;

void main()
{
	vec3 Color = texture(TextureSampler, FragmentUV).rgb * FragmentColor.rgb;
	vec3 DiffuseColor = Color * MaterialDiffuseColor;
	vec3 AmbientColor = Color * MaterialAmbientColor;
	vec3 SpectularColor = MaterialSpecularColor;
	
	vec3 LightDir = normalize(vec3(-1.0f, 1.0f, 1.0f));
	vec3 LightColor = vec3(0.8f, 0.8f, 0.8f);
	
	// Eye vector (towards the camera)
	vec3 EyeDir = normalize(EyePos - FragmentPos);
	
	float CosTheta = clamp(dot(FragmentNormal, LightDir), 0.0f, 1.0f);
	
	// Direction in which the triangle reflects the light
	vec3 ReflectionDir = reflect(-LightDir, FragmentNormal);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float CosAlpha = CosTheta > 0.0f ? clamp(dot(EyeDir, ReflectionDir), 0.0f, 1.0f) : 0.0f;

	PixelColor = vec4(clamp(
		AmbientColor +
		DiffuseColor * LightColor * CosTheta +
		SpectularColor * LightColor * pow(CosAlpha, MaterialShininess), 0.0f, 1.0f),
		FragmentColor.a);
}
