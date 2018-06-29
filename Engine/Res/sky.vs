cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 viewDir : TEXCOORD0;
};

PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    input.position.w = 1.0f;

    output.position  = mul(worldMatrix, input.position);
    output.position  = mul(viewMatrix, output.position);
    output.position = mul(projectionMatrix, output.position);

	output.viewDir = -input.position.xyz;
    
    return output;
}