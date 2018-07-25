SubShader {
	Desc {
		CompileTarget: { d3d10 d3d11 }
	}
	Pass {
		State {
			zwrite on
			ztest lequal
		}

		Shader {
			#vert VertMain
			#frag FragMain
			#code [

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
							textureColor.rgb *= 0.3f;

	    			return textureColor;
				}

			]
		}
	}
}
SubShader {
	Desc {
		CompileTarget: { d3d9 }
	}
	Pass {
		State {
			zwrite on
			ztest lequal
		}

		Shader {
			#vert VertMain
			#frag FragMain
			#code [

				matrix g_worldMatrix;
				matrix g_viewMatrix;
				matrix g_projectionMatrix;

				matrix g_shadowView;
				matrix g_shadowProj;

				float4 g_shadowParams;

				struct VS_INPUT
				{
				    float3 position : POSITION;
				    float2 uv       : TEXCOORD0;
				};

				struct VS_OUTPUT
				{
				    float4 position : POSITION;
				    float2 uv  : TEXCOORD0;
					float3 shadowUV:TEXCOORD1;
				};

				sampler shaderTexture;
				sampler g_shadowMap;

				VS_OUTPUT VertMain(VS_INPUT input)
				{
				    VS_OUTPUT output = (VS_OUTPUT)0;

	    			float4 pos = float4(input.position, 1.0f);

	    			output.position = mul(pos, g_worldMatrix);

						float4 shadowPos = mul(output.position, g_shadowView);

						output.shadowUV.z = shadowPos.z * g_shadowParams.w;
						shadowPos = mul(shadowPos, g_shadowProj);

						output.shadowUV.x = shadowPos.x / shadowPos.w * 0.5 + 0.5;
						output.shadowUV.y = -(shadowPos.y / shadowPos.w * 0.5)+0.5;

	    			output.position = mul(output.position, g_viewMatrix);
	    			output.position = mul(output.position, g_projectionMatrix);

	    			output.uv = input.uv;

	    			return output;
				}

				float4 FragMain(VS_OUTPUT input) : SV_TARGET
				{
				    float4 col = tex2D(shaderTexture,      input.uv);
				    float4 depthColor = tex2D(g_shadowMap, input.shadowUV.xy);

						if(input.shadowUV.z - depthColor.r > 0.01)
							col.rgb *= 0.3;
						return col;
				}
			]
		}
	}
}
