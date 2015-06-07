//////////////////////////////////
// Filename: psTranslate.hlsli
//////////////////////////////////

//////////////////////////////////
// Globals
//--------------------------------
Texture2D shaderTexture;
SamplerState sampleType;

cbuffer TranslationBuffer
{
	float textureTranslation;
};

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
float4 TranslatePixelShader(PixelInputType input) : SV_TARGET
{
	// Change the position tgat we sample the texture from.
	input.tex.x += textureTranslation;

	return shaderTexture.Sample(sampleType, input.tex);
}