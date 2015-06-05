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
};

/////////////////////////////////
// Pixel Shader
//-------------------------------
float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;

	// Sample the input texture.
	textureColor = shaderTexture.Sample(sampleType, input.tex);

	return textureColor;
}