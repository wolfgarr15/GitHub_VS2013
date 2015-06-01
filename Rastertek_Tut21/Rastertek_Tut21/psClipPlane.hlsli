//////////////////////////////////
// Filename: psClipPlane.hlsli
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
	float clip : SV_ClipDistance0;
};

/////////////////////////////////
// Pixel Shader
//-------------------------------
float4 ClipPlanePixelShader(PixelInputType input) : SV_TARGET
{
	return shaderTexture.Sample(sampleType, input.tex);
}