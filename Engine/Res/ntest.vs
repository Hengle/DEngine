////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;


	// Change the position vector to be 4 units for proper matrix calculations.
    //input.position.w = 1.0f;
	float4 vpos = float4(input.position.xyz, 1.0f);

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position  = mul(vpos , worldMatrix);
    output.position  = mul(output.position , viewMatrix);
    output.position = mul(output.position , projectionMatrix);

	// Store the input color for the pixel shader to use.
    output.normal = normalize(input.normal)*0.5f+0.5f;

    return output;
}