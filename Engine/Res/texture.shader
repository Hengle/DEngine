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

				    output.position  = mul(g_worldMatrix, input.position);
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

	    			output.position = mul(pos, g_worldMatrix);
	    			output.position = mul(output.position, g_viewMatrix);
	    			output.position = mul(output.position, g_projectionMatrix);

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

					layout(location = 0) in vec3 input_position;
					layout(location = 1) in vec2 input_texcoord0;

					uniform mat4 g_worldMatrix;
					uniform mat4 g_viewMatrix;
					uniform mat4 g_projectionMatrix;

					void main(){
 						gl_Position = g_worldMatrix * vec4(input_position,1);
						gl_Position = g_viewMatrix * gl_Position;
						gl_Position = g_projectionMatrix * gl_Position;
						uv = input_texcoord0;
					}

				]
				#frag [
					#version 330 core

					in vec2 uv;

					uniform sampler2D shaderTexture;
					uniform sampler2D shaderTexture2;

					out vec4 color;
					void main(){
						color = texture(shaderTexture, uv) + texture(shaderTexture2, uv);
					}
				]
			]
		}
	}
}