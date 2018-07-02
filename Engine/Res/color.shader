SubShader {
	Desc {
		CompileTarget: { d3d10 d3d11 }
	}
	Pass {
		Tags {
			VertexFunc:   VertMain
			PixelFunc:  FragMain
		}
		State {
			zwrite on
			ztest lequal
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
SubShader {
	Desc {
		CompileTarget: { d3d9 }
	}
	Pass {
		Tags {
			VertexFunc:   VertMain
			PixelFunc:  FragMain
		}
		State {
			zwrite on
			ztest lequal
		}

		Shader [

			matrix worldMatrix;
			matrix viewMatrix;
			matrix projectionMatrix;

			struct VS_INPUT
			{
			    float3 position : POSITION;
	  			float4 color    : COLOR;
			};

			struct VS_OUTPUT
			{
			    float4 position : POSITION;
	    		float4 color  : COLOR;
			};

			VS_OUTPUT VertMain(VS_INPUT input)
			{
			    VS_OUTPUT output = (VS_OUTPUT)0;

			    float4 pos = float4(input.position, 1.0f);

    			output.position = mul(pos, worldMatrix);
    			output.position = mul(output.position, viewMatrix);
    			output.position = mul(output.position, projectionMatrix);

    			output.color = input.color;
    
    			return output;
			}

			float4 FragMain(VS_OUTPUT input) : SV_TARGET
			{
			    return input.color;
			}
		]
	}
}