//////////////////////////////////
// Filename: vsReflection.hlsli
//////////////////////////////////

//////////////////////////////////
// Globals
//--------------------------------
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ReflectionBuffer
{
	matrix reflectionMatrix;
};

////////////////////////////////
// Typedefs
//-------------------------------
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 reflectionPosition : TEXCOORD1;
};

/////////////////////////////////
// Vertex Shader
//-------------------------------
PixelInputType ReflectionVertexShader(VertexInputType input)
{
	PixelInputType output;
	matrix reflectProjWorld;

	// Change the position to be 4 units.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the input texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Create the reflection projection world matrix.
	reflectProjWorld = mul(reflectionMatrix, projectionMatrix);
	reflectProjWorld = mul(worldMatrix, reflectProjWorld);

	// Calculate the reflection position against the reflectProjWorld matrix.
	output.reflectionPosition = mul(input.position, reflectProjWorld);

	return output;
}