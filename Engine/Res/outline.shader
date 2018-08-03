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
					matrix g_engineWorldMatrix;
					matrix g_engineViewMatrix;
					matrix g_engineProjectionMatrix;
				};

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

				Texture2D shaderTexture;
				SamplerState SampleType;

				PixelInputType VertMain(VertexInputType input)
				{
				    PixelInputType output;


					// Change the position vector to be 4 units for proper matrix calculations.
				    input.position.w = 1.0f;

					// Calculate the position of the vertex against the world, view, and projection matrices.

				    output.position  = mul(g_engineWorldMatrix, input.position);
				    output.position  = mul(g_engineViewMatrix, output.position);
				    output.position = mul(g_engineProjectionMatrix, output.position);

					// Store the texture coordinates for the pixel shader.
					output.tex = input.tex;

				    return output;
				}

				float4 FragMain(PixelInputType input) : SV_TARGET
				{
				    float4 textureColor;


	    			// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	    			textureColor = shaderTexture.Sample(SampleType, input.tex);

	    			return textureColor;
				}
			]
		}
	}
	Pass {
		State {
			zwrite on
			ztest lequal
			cull front
		}

		Shader {
			#vert VertMain
			#frag FragMain
			#code [
				cbuffer MatrixBuffer
				{
					matrix g_engineWorldMatrix;
					matrix g_engineViewMatrix;
					matrix g_engineProjectionMatrix;
				};

				struct VertexInputType
				{
				    float4 position : POSITION;
				    float3 normal : NORMAL;
				};

				struct PixelInputType
				{
				    float4 position : SV_POSITION;
				};

				float outline;
				float4 outlinecolor;

				PixelInputType VertMain(VertexInputType input)
				{
				    PixelInputType output;

				    input.position.w = 1.0f;
						input.position.xyz += input.normal.xyz*outline;

				    output.position  = mul(g_engineWorldMatrix, input.position);
				    output.position  = mul(g_engineViewMatrix, output.position);
				    output.position = mul(g_engineProjectionMatrix, output.position);

				    return output;
				}

				float4 FragMain(PixelInputType input) : SV_TARGET
				{
	    			return outlinecolor;
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
				matrix g_engineWorldMatrix;
				matrix g_engineViewMatrix;
				matrix g_engineProjectionMatrix;

				struct VS_INPUT
				{
				    float3 position : POSITION;
				    float2 uv       : TEXCOORD0;
				};

				struct VS_OUTPUT
				{
				    float4 position : POSITION;
				    float2 uv  : TEXCOORD0;
				};

				sampler shaderTexture;

				VS_OUTPUT VertMain(VS_INPUT input)
				{
				    VS_OUTPUT output = (VS_OUTPUT)0;

	    			float4 pos = float4(input.position, 1.0f);

	    			output.position = mul(pos, g_engineWorldMatrix);
	    			output.position = mul(output.position, g_engineViewMatrix);
	    			output.position = mul(output.position, g_engineProjectionMatrix);

	    			output.uv = input.uv;

	    			return output;
				}

				float4 FragMain(VS_OUTPUT input) : SV_TARGET
				{
				    return tex2D(shaderTexture,      input.uv);
				}
			]
		}
	}
	Pass {
		State {
			zwrite on
			ztest lequal
			cull front
		}

		Shader {
			#vert VertMain
			#frag FragMain
			#code [

				matrix g_engineWorldMatrix;
				matrix g_engineViewMatrix;
				matrix g_engineProjectionMatrix;

				struct VS_INPUT
				{
				    float3 position : POSITION;
				    float3 normal   : NORMAL;
				};

				struct VS_OUTPUT
				{
				    float4 position : POSITION;
				};

				float outline;
				float4 outlinecolor;

				VS_OUTPUT VertMain(VS_INPUT input)
				{
				    VS_OUTPUT output = (VS_OUTPUT)0;

				    input.position.xyz += input.normal.xyz*outline;
	    			float4 pos = float4(input.position, 1.0f);

	    			output.position = mul(pos, g_engineWorldMatrix);
	    			output.position = mul(output.position, g_engineViewMatrix);
	    			output.position = mul(output.position, g_engineProjectionMatrix);

	    			return output;
				}

				float4 FragMain(VS_OUTPUT input) : SV_TARGET
				{
				    return outlinecolor;
				}
			]
		}
	}
}
