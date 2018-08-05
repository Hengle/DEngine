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

				//cbuffer ShadowBuffer
				//{
					matrix g_engineShadowView;
					matrix g_engineShadowProj;
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
				Texture2D g_engineShadowMap;
				SamplerState SampleType;

				float4 g_engineShadowParams;

				PixelInputType VertMain(VertexInputType input)
				{
				    PixelInputType output;


					// Change the position vector to be 4 units for proper matrix calculations.
				    input.position.w = 1.0f;

					// Calculate the position of the vertex against the world, view, and projection matrices.

				    output.position  = mul(g_engineWorldMatrix, input.position);

						float4 shadowPos = mul(g_engineShadowView, output.position);

						output.shadowUV.z = shadowPos.z * g_engineShadowParams.w;
						shadowPos = mul(g_engineShadowProj, shadowPos);

						output.shadowUV.x = shadowPos.x / shadowPos.w * 0.5f + 0.5f;
						output.shadowUV.y = -(shadowPos.y / shadowPos.w * 0.5f)+0.5f;

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
						float4 depthColor = g_engineShadowMap.Sample(SampleType, input.shadowUV.xy);

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

				matrix g_engineWorldMatrix;
				matrix g_engineViewMatrix;
				matrix g_engineProjectionMatrix;

				matrix g_engineShadowView;
				matrix g_engineShadowProj;

				float4 g_engineShadowParams;

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
				sampler g_engineShadowMap;

				VS_OUTPUT VertMain(VS_INPUT input)
				{
				    VS_OUTPUT output = (VS_OUTPUT)0;

	    			float4 pos = float4(input.position, 1.0f);

	    			output.position = mul(pos, g_engineWorldMatrix);

						float4 shadowPos = mul(output.position, g_engineShadowView);

						output.shadowUV.z = shadowPos.z * g_engineShadowParams.w;
						shadowPos = mul(shadowPos, g_engineShadowProj);

						output.shadowUV.x = shadowPos.x / shadowPos.w * 0.5 + 0.5;
						output.shadowUV.y = -(shadowPos.y / shadowPos.w * 0.5)+0.5;

	    			output.position = mul(output.position, g_engineViewMatrix);
	    			output.position = mul(output.position, g_engineProjectionMatrix);

	    			output.uv = input.uv;

	    			return output;
				}

				float4 FragMain(VS_OUTPUT input) : SV_TARGET
				{
				    float4 col = tex2D(shaderTexture,      input.uv);
				    float4 depthColor = tex2D(g_engineShadowMap, input.shadowUV.xy);

						if(input.shadowUV.z - depthColor.r > 0.01)
							col.rgb *= 0.3;
						return col;
				}
			]
		}
	}
}
SubShader {
	Desc {
		CompileTarget: { opengl }
	}
	Pass {
		State {
			zwrite on
			ztest lequal
		}

		Shader {
			#code [
				#vert [
					#version 330 core

					out vec4 gl_Position;
					out vec2 uv;
					out vec3 shadowUV;

					layout(location = 0) in vec3 input_position;
					layout(location = 1) in vec2 input_texcoord0;

					uniform mat4 g_engineWorldMatrix;
					uniform mat4 g_engineViewMatrix;
					uniform mat4 g_engineProjectionMatrix;

					uniform mat4 g_engineShadowView;
					uniform mat4 g_engineShadowProj;

					uniform vec4 g_engineShadowParams;

					void main(){
 						gl_Position = g_engineWorldMatrix * vec4(input_position,1);

 						vec4 shadowPos = g_engineShadowView * gl_Position;
 						shadowUV.z = -shadowPos.z / g_engineShadowParams.z;
 						shadowPos = g_engineShadowProj * shadowPos;
 						shadowUV.x = shadowPos.x / shadowPos.w * 0.5 + 0.5;
 						shadowUV.y = (shadowPos.y / shadowPos.w * 0.5)+0.5;

						gl_Position = g_engineViewMatrix * gl_Position;
						gl_Position = g_engineProjectionMatrix * gl_Position;
						uv = input_texcoord0;
					}

				]
				#frag [
					#version 330 core

					in vec2 uv;
					in vec3 shadowUV;

					uniform sampler2D shaderTexture;
					uniform sampler2D g_engineShadowMap;

					out vec4 color;
					void main(){
						color = texture(shaderTexture,uv);
						vec4 depthColor = texture(g_engineShadowMap, shadowUV.xy);
						if(shadowUV.z - depthColor.r > 0.01)
							color.rgb *= 0.3;
					}
				]
			]
		}
	}
}