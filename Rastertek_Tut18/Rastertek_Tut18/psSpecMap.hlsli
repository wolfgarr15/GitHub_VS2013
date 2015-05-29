//////////////////////////////////
// Filename: psSpecMap.hlsli
//////////////////////////////////

//////////////////////////////////
// Globals
//--------------------------------
Texture2D shaderTextures[3];
SamplerState sampleType;

cbuffer LightBuffer
{
	float4 diffuseColor;
	float4 specularColor;
	float3 lightDirection;
	float specularPower;
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
	float3 viewDirection : TEXCOORD1;
};

/////////////////////////////////
// Pixel Shader
//-------------------------------
float4 SpecMapPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 bumpMap;
	float3 bumpNormal;
	float3 lightDir;
	float lightIntensity;
	float4 color;
	float4 specIntensity;
	float3 reflection;
	float4 specular;



	// *** Begin bump map calculations... ***

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

	// Determine the color based on the bump map weighted diffuse color and texture color.
	color = saturate(diffuseColor * lightIntensity) * textureColor;



	// *** Begin specular lighting calculations... ***

	if (lightIntensity > 0.0f)
	{
		// Sample the specular map.
		specIntensity = shaderTextures[2].Sample(sampleType, input.tex);

		// Calculate the reflection vector.
		reflection = normalize(2 * lightIntensity * bumpNormal - lightDir);

		// Determine the amount of specular light.
		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

		// Use the specular map to determine the intensity of the specular lighting at this pixel.
		specular = specular * specIntensity;

		// Add the specular component to the total color.
		color = saturate(color + specular);
	}

	return color;
}