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

			//cbuffer ShadowBuffer
			//{
				matrix g_shadowView;
				matrix g_shadowProj;
			//};

			struct VertexInputType
			{
			    float4 position : POSITION;
			    float2 tex : TEXCOORD0;
			};

			struct PixelInputType
			{
			    float4 position : SV_POSITION;
			    float2 tex : TEXCOORD0;
					float3 shadowUV:TEXCOORD1;
			};

			Texture2D shaderTexture;
			Texture2D g_shadowMap;
			SamplerState SampleType;

			float4 g_shadowParams;

			PixelInputType VertMain(VertexInputType input)
			{
			    PixelInputType output;


				// Change the position vector to be 4 units for proper matrix calculations.
			    input.position.w = 1.0f;

				// Calculate the position of the vertex against the world, view, and projection matrices.

			    output.position  = mul(g_worldMatrix, input.position);

					float4 shadowPos = mul(g_shadowView, output.position);

					output.shadowUV.z = shadowPos.z * g_shadowParams.w;
					shadowPos = mul(g_shadowProj, shadowPos);

					output.shadowUV.x = shadowPos.x / shadowPos.w * 0.5f + 0.5f;
					output.shadowUV.y = -(shadowPos.y / shadowPos.w * 0.5f)+0.5f;

			    output.position  = mul(g_viewMatrix, output.position);
			    output.position = mul(g_projectionMatrix, output.position);

				// Store the texture coordinates for the pixel shader.
				output.tex = input.tex;

			    return output;
			}

			float4 FragMain(PixelInputType input) : SV_TARGET
			{
			    float4 textureColor;

    			// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    			textureColor = shaderTexture.Sample(SampleType, input.tex);
					float4 depthColor = g_shadowMap.Sample(SampleType, input.shadowUV.xy);

					if(input.shadowUV.z - depthColor.r > 0.0001f)
						textureColor.rgb *= 0.0f;

    			return textureColor;
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

			matrix g_shadowView;
			matrix g_shadowProj;

			struct VS_INPUT
			{
			    float3 position : POSITION;
			    float2 uv       : TEXCOORD0;
			};

			struct VS_OUTPUT
			{
			    float4 position : POSITION;
			    float2 uv  : TEXCOORD0;
					float4 shadowProj:TEXCOORD1;
			};

			sampler shaderTexture;
			sampler g_shadowMap;

			VS_OUTPUT VertMain(VS_INPUT input)
			{
			    VS_OUTPUT output = (VS_OUTPUT)0;

    			float4 pos = float4(input.position, 1.0f);

    			output.position = mul(pos, g_worldMatrix);

					output.shadowProj = mul(output.position, g_shadowView);
					output.shadowProj = mul(output.shadowProj, g_shadowProj);

    			output.position = mul(output.position, g_viewMatrix);
    			output.position = mul(output.position, g_projectionMatrix);

    			output.uv = input.uv;

    			return output;
			}

			float4 FragMain(VS_OUTPUT input) : SV_TARGET
			{
				float2 shadowUv;
				shadowUv.x = input.shadowProj.x / input.shadowProj.w * 0.5f + 0.5f;
				shadowUv.y = -(input.shadowProj.y / input.shadowProj.w  * 0.5f)+0.5f;
				  float4 depthColor = tex2D(g_shadowMap, shadowUv);
			    //return tex2D(shaderTexture,      input.uv);
					return depthColor;
			}
		]
	}
}
