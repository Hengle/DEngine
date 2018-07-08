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
				matrix g_worldMatrix;
				matrix g_viewMatrix;
				matrix g_projectionMatrix;
			};

			struct VertexInputType
			{
			    float3 position : POSITION;
			};

			struct PixelInputType
			{
			    float4 position : SV_POSITION;
			    float depth     : TEXCOORD0;
			};

			float4 g_shadowParams;

			PixelInputType VertMain(VertexInputType input)
			{
			    PixelInputType output;

				float4 vpos = float4(input.position.xyz, 1.0f);

			    output.position  = mul(g_worldMatrix, vpos);
			    output.position  = mul(g_viewMatrix, output.position);

			    output.depth = output.position.z/output.position.w   * g_shadowParams.w;

			    output.position = mul(g_projectionMatrix, output.position);

			    return output;
			}

			float4 FragMain(PixelInputType input) : SV_TARGET
			{
			    return float4(input.depth, input.depth, input.depth, 1.0f);
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

			matrix g_worldMatrix;
			matrix g_viewMatrix;
			matrix g_projectionMatrix;

			float4 g_shadowParams;

			struct VS_INPUT
			{
			    float3 position : POSITION;
			};

			struct VS_OUTPUT
			{
			    float4 position : POSITION;
	    		float depth     : TEXCOORD0;
			};

			VS_OUTPUT VertMain(VS_INPUT input)
			{
			    VS_OUTPUT output = (VS_OUTPUT)0;

			    float4 pos = float4(input.position, 1.0f);

    			output.position = mul(pos, g_worldMatrix);
    			output.position = mul(output.position, g_viewMatrix);

    			output.depth = output.position.z * g_shadowParams.w;

    			output.position = mul(output.position, g_projectionMatrix);
    
    			return output;
			}

			float4 FragMain(VS_OUTPUT input) : SV_TARGET
			{
			    return float4(input.depth, input.depth, input.depth, 1.0f);
			}
		]
	}
}