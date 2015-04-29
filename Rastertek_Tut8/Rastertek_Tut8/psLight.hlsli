//////////////////////////////////////
// Filename: light.psh
//////////////////////////////////////

//////////////////////////////////////
// Globals
//-----------------------------------
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
};

/////////////////////////////////////
// Typedefs
//-----------------------------------
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 viewDirection : TEXCOORD1;
	float3 normal : NORMAL;
};

////////////////////////////////////
// Pixel Shader
//----------------------------------
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
	float3 reflection;
	float4 specular;
	float specMagnitude;

	// Sample the pixel color from the texture.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Set the default output color to the ambient light value for all pixels.
	color = ambientColor;

	// Initialize the specular color.
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on the pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));

	if(lightIntensity > 0.0f)
	{
		// Determine the amount of diffuse color.
		color += (diffuseColor * lightIntensity);

		// Saturate the ambient and diffuse color.
		color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal, and light direction.
		reflection = normalize(2 * lightIntensity * input.normal - lightDir);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		 specMagnitude = dot(reflection, input.viewDirection);
		 specular = pow(saturate(specMagnitude), specularPower);
	}

	// Multiply the light color by the texture color to get the textured result.
	color = color * textureColor;

	// Saturate the combined color and specular to get the final pixel color.
	color = saturate(color + specular);

	return color;
}