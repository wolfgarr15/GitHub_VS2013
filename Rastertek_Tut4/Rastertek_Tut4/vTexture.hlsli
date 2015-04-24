//////////////////////////////////
// Filename: vTexture.hlsli
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
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

/////////////////////////////////
// Vertex Shader
//-------------------------------
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the input texture coordinates for the pixel shader.
	output.tex = input.tex;

	return output;
}