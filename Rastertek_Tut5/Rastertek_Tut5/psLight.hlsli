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
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

/////////////////////////////////
// Typedefs
//-------------------------------
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
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

	// Sample the input texture.
	textureColor = shaderTexture.Sample(sampleType, input.tex);

	// Invert the light direction for calculations.
	lightDir = -1.0f * lightDirection;

	// Calculate the amount of diffuse light on the pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));

	// Determine the final amount of diffuse color.
	color = saturate(diffuseColor * lightIntensity);

	// Multiply the texture color and diffuse color to get the final pixel color.
	color = color * textureColor;

	return color;
}