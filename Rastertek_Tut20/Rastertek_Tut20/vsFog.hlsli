//////////////////////////////////
// Filename: vsFog.hlsli
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

cbuffer FogBuffer
{
	float fogStart;
	float fogEnd;
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
	float fogFactor : FOG;
};

/////////////////////////////////
// Vertex Shader
//-------------------------------
PixelInputType FogVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 viewPosition;

	// Change the position to be 4 units.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the input texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the view position.
	viewPosition = mul(input.position, worldMatrix);
	viewPosition = mul(viewPosition, viewMatrix);

	// Calculate the fog factor at this position.
	output.fogFactor = saturate((fogEnd - viewPosition.z) / (fogEnd - fogStart));

	return output;
}