SubShader {
	Tags {
		CompileTarget: { d3d10 d3d11 }
		VertexFunc:   VertMain
		PixelFunc:  FragMain
	}
	Pass {
		State {
			zwrite off
			ztest lequal
			cull off
		}

		Shader [

			cbuffer MatrixBuffer
			{
				matrix worldMatrix;
				matrix viewMatrix;
				matrix projectionMatrix;
			};

			struct VertexInputType
			{
			    float3 position : POSITION;
			    float4 color    : COLOR;
			};

			struct PixelInputType
			{
			    float4 position : SV_POSITION;
			    float4 color : COLOR;
			};

			PixelInputType VertMain(VertexInputType input)
			{
			    PixelInputType output;

				float4 vpos = float4(input.position.xyz, 1.0f);

			    output.position  = mul(worldMatrix, vpos);
			    output.position  = mul(viewMatrix, output.position);
			    output.position = mul(projectionMatrix, output.position);

			    output.color = input.color;

			    return output;
			}

			float4 FragMain(PixelInputType input) : SV_TARGET
			{
			    return input.color;
			}
		]
	}
}