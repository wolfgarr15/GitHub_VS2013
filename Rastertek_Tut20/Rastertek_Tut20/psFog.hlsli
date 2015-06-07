//////////////////////////////////
// Filename: psTexture.hlsli
//////////////////////////////////

//////////////////////////////////
// Globals
//--------------------------------
Texture2D shaderTexture;
SamplerState sampleType;

/////////////////////////////////
// Typedefs
//-------------------------------
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float fogFactor : FOG;
};

/////////////////////////////////
// Pixel Shader
//-------------------------------
float4 FogPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 fogColor;
	float4 finalColor;

	// Sample the input texture.
	textureColor = shaderTexture.Sample(sampleType, input.tex);

	// Set the color of the fog to grey.
	fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);

	// Calculate the final color using the linear fog equation.
	finalColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;

	return finalColor;
}