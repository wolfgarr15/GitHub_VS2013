//////////////////////////////////
// Filename: psMultitexture.hlsli
//////////////////////////////////

//////////////////////////////////
// Globals
//--------------------------------
Texture2D shaderTexture[2];
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
float4 MultitexturePixelShader(PixelInputType input) : SV_TARGET
{
	float4 color1;
	float4 color2;
	float4 blendColor;
	float gammaFactor;

	// Set the gamma correction factor (2.0 is approx. the gamma setting of most modern monitors).
	gammaFactor = 2.0f;

	// Sample the first texture.
	color1 = shaderTexture[0].Sample(sampleType, input.tex);

	// Sample the second texture.
	color2 = shaderTexture[1].Sample(sampleType, input.tex);

	// Blend the two textures.
	blendColor = color1 * color2 * gammaFactor;

	// Saturate the final color.
	blendColor = saturate(blendColor);

	return blendColor;
}