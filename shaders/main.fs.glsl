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
uniform vec3 MissilePos;
uniform vec3 MissileColor;

void main()
{
	vec3 Color = texture(TextureSampler, FragmentUV).rgb * FragmentColor.rgb;
	vec3 DiffuseColor = Color * MaterialDiffuseColor;
	vec3 AmbientColor = Color * MaterialAmbientColor;
	vec3 SpectularColor = MaterialSpecularColor;
	
	// Directional light - moon
	vec3 MoonDir = normalize(vec3(-1.0f, 1.0f, 1.0f));
	vec3 MoonColor = vec3(0.8f, 0.8f, 0.8f);
	
	// Eye vector (towards the camera)
	vec3 EyeDir = normalize(EyePos - FragmentPos);
	
	// Direction in which the triangle reflects the moon light
	vec3 ReflectionDir = reflect(-MoonDir, FragmentNormal);
	
	float CosTheta = clamp(dot(FragmentNormal, MoonDir), 0.0f, 1.0f);
	float CosAlpha = CosTheta > 0.0f ? clamp(dot(EyeDir, ReflectionDir), 0.0f, 1.0f) : 0.0f;

	// Color based on moon light
	vec3 ColorMoon =
		DiffuseColor * MoonColor * CosTheta +
		SpectularColor * MoonColor * pow(CosAlpha, MaterialShininess);
	
	// Direction in which the triangle reflects the moon light
	vec3 MissileDir = normalize(MissilePos - FragmentPos);
	float MissileDist = distance(MissilePos, FragmentPos);
	ReflectionDir = reflect(-MissileDir, FragmentNormal);
	CosTheta = clamp(dot(FragmentNormal, MissileDir), 0.0f, 1.0f);
	CosAlpha = CosTheta > 0.0f ? clamp(dot(EyeDir, ReflectionDir), 0.0f, 1.0f) : 0.0f;

	// Color based on moon light
	vec3 ColorMissile =
		(DiffuseColor * MissileColor * CosTheta +
		SpectularColor * MissileColor * pow(CosAlpha, MaterialShininess)) / (MissileDist*MissileDist);
	
	PixelColor = vec4(clamp(AmbientColor + ColorMoon + ColorMissile, 0.0f, 1.0f), FragmentColor.a);
}
