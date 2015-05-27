//////////////////////////////////
// Filename: psAlphaMap.hlsli
//////////////////////////////////

//////////////////////////////////
// Globals
//--------------------------------
Texture2D shaderTextures[3];
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
float4 AlphaMapPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color1;
	float4 color2;
	float4 alphaValue;
	float4 blendColor;

	// Sample the base texture.
	color1 = shaderTextures[0].Sample(sampleType, input.tex);

	// Sample the second texture.
	color2 = shaderTextures[1].Sample(sampleType, input.tex);

	// Sample the alpha map texture.
	alphaValue = shaderTextures[2].Sample(sampleType, input.tex);

	// Blend the colors weighted by the alpha value.
	blendColor = (alphaValue * color1) + ((1.0f - alphaValue) * color2);

	// Saturate the blended color.
	blendColor = saturate(blendColor);

	return blendColor;
}