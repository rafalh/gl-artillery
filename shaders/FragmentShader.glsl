#version 330 core

// Interpolated values from the Vertex Shaders
in vec3 FragmentPos;
in vec3 FragmentNormal;
in vec2 FragmentUV;
in vec3 FragmentColor;

// Ouput data
out vec3 PixelColor;

// Values that stay constant for the whole rendering
uniform sampler2D TextureSampler;
uniform vec3 EyePos;

void main()
{
	vec3 DiffuseColor = texture(TextureSampler, FragmentUV).rgb * FragmentColor;
	vec3 SpectularColor = DiffuseColor;
	
	vec3 AmbientColor = vec3(0.2f, 0.2f, 0.2f);
	vec3 LightDir = normalize(vec3(0.2f, 0.8f, 0.1f));
	vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);
	
	// Eye vector (towards the camera)
	vec3 EyeDir = normalize(EyePos - FragmentPos);
	
	float CosTheta = clamp(dot(FragmentNormal, LightDir), 0, 1);
	
	// Direction in which the triangle reflects the light
	vec3 ReflectionDir = reflect(-LightDir, FragmentNormal);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float CosAlpha = clamp(dot(EyeDir, ReflectionDir), 0.0f, 1.0f);

	PixelColor =
		// Ambient : simulates indirect lighting
		AmbientColor +
		// Diffuse : "color" of the object
		DiffuseColor * LightColor * CosTheta;
		// Specular : reflective highlight, like a mirror
		//SpectularColor * LightColor * pow(CosAlpha, 16.0f);
}
