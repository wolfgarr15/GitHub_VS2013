//////////////////////////////////
// Filename: psTransparent.hlsli
//////////////////////////////////

//////////////////////////////////
// Globals
//--------------------------------
Texture2D shaderTexture;
SamplerState sampleType;

cbuffer TransparentBuffer
{
	float blendAmount;
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
float4 TransparentPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;

	// Sample the input texture.
	color = shaderTexture.Sample(sampleType, input.tex);

	// Set the alpha value of this pixel to the blend amount.
	color.a = blendAmount;

	return color;
}