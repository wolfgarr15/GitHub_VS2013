//////////////////////////////////
// Filename: psBumpMap.hlsli
//////////////////////////////////

//////////////////////////////////
// Globals
//--------------------------------
Texture2D shaderTextures[2];
SamplerState sampleType;

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
};

/////////////////////////////////
// Typedefs
//-------------------------------
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

/////////////////////////////////
// Pixel Shader
//-------------------------------
float4 BumpMapPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 bumpMap;
	float3 bumpNormal;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	// Sample the input texture.
	textureColor = shaderTextures[0].Sample(sampleType, input.tex);

	// Sample the bump map.
	bumpMap = shaderTextures[1].Sample(sampleType, input.tex);

	// Expand the range of the normal value from [0, +1] to [-1, +1].
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	// Calculate and normalize the normal from the bump map data.
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	bumpNormal = normalize(bumpNormal);

	// Invert the light direction for calculations.
	lightDir = -1.0f * lightDirection;

	// Calculate the light intensity on this pixel based on the bump map normal.
	lightIntensity = saturate(dot(bumpNormal, lightDir));

	// Determine the final color.
	color = saturate(diffuseColor * lightIntensity) * textureColor;

	return color;
}