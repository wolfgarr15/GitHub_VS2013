//////////////////////////////////
// Filename: psLightMap.hlsli
//////////////////////////////////

//////////////////////////////////
// Globals
//--------------------------------
Texture2D shaderTextures[2];
SamplerState sampleType;

/////////////////////////////////
// Typedefs
//-------------------------------
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

/////////////////////////////////
// Pixel Shader
//-------------------------------
float4 LightMapPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;
	float4 lightColor;
	float4 finalColor;

	// Sample the texture.
	color = shaderTextures[0].Sample(sampleType, input.tex);

	// Sample the light map texture.
	lightColor = shaderTextures[1].Sample(sampleType, input.tex);

	// Blend the two pixel colors.
	finalColor = color * lightColor;

	return finalColor;
}