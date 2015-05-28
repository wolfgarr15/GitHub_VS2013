//////////////////////////////////
// Filename: vsBumpMap.hlsli
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

////////////////////////////////
// Typedefs
//-------------------------------
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

/////////////////////////////////
// Vertex Shader
//-------------------------------
PixelInputType BumpMapVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Change the position to be 4 units.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the input texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normalized normal vector.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	// Calculate the normalized tangent vector.
	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	// Calculate the normalized binormal vector.
	output.binormal = mul(input.binormal, (float3x3)worldmatrix);
	output.binormal = normalize(input.binormal, (float3x3)worldMatrix);

	return output;
}