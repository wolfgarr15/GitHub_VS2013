//////////////////////////////////
// Filename: psLight.hlsli
//////////////////////////////////

//////////////////////////////////
// Globals
//--------------------------------
Texture2D shaderTexture;
SamplerState sampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
};

/////////////////////////////////
// Typedefs
//-------------------------------
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

/////////////////////////////////
// Pixel Shader
//-------------------------------
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
	float3 reflection;
	float4 specular;

	// Sample the input texture.
	textureColor = shaderTexture.Sample(sampleType, input.tex);

	// Set the default output color to the ambient light value for all pixels.
	color = ambientColor;

	// Initialize the specular color.
	specular = float4(0.0f, 0.0f, 0.0f, 1.0f);

	// Invert the light direction for calculations.
	lightDir = -1.0f * lightDirection;

	// Calculate the amount of diffuse light on the pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		// Determine the final amount of diffuse color.
		color += saturate(diffuseColor * lightIntensity);

		// Saturate the ambient and diffuse color.
		color = saturate(color);

		// Calculate the reflection based on the light intensity, normal vector, and light direction.
		reflection = normalize(2.0f * input.normal - lightDirection);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		specular = pow(saturate(lightIntensity * dot(reflection, input.viewDirection)), specularPower);
	}

	// Determine the final color.
	color = saturate(color * textureColor + specular);

	return color;
}