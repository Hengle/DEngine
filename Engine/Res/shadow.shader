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
				    float3 position : POSITION;
				};

				struct PixelInputType
				{
				    float4 position : SV_POSITION;
				    float depth     : TEXCOORD0;
				};

				float4 g_engineShadowParams;

				PixelInputType VertMain(VertexInputType input)
				{
				    PixelInputType output;

					float4 vpos = float4(input.position.xyz, 1.0f);

				    output.position  = mul(g_engineWorldMatrix, vpos);
				    output.position  = mul(g_engineViewMatrix, output.position);

				    output.depth = output.position.z/output.position.w   * g_engineShadowParams.w;

				    output.position = mul(g_engineProjectionMatrix, output.position);

				    return output;
				}

				float4 FragMain(PixelInputType input) : SV_TARGET
				{
				    return float4(input.depth, input.depth, input.depth, 1.0f);
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

				float4 g_engineShadowParams;

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

	    			output.position = mul(pos, g_engineWorldMatrix);
	    			output.position = mul(output.position, g_engineViewMatrix);

	    			output.depth = output.position.z * g_engineShadowParams.w;

	    			output.position = mul(output.position, g_engineProjectionMatrix);
	    
	    			return output;
				}

				float4 FragMain(VS_OUTPUT input) : SV_TARGET
				{
				    return float4(input.depth, input.depth, input.depth, 1.0f);
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
					out float depth;

					layout(location = 0) in vec3 input_position;

					uniform mat4 g_engineWorldMatrix;
					uniform mat4 g_engineViewMatrix;
					uniform mat4 g_engineProjectionMatrix;

					uniform vec4 g_engineShadowParams;

					void main(){
 						gl_Position = g_engineWorldMatrix * vec4(input_position,1);
						gl_Position = g_engineViewMatrix * gl_Position;


						depth = -gl_Position.z/g_engineShadowParams.z;

						gl_Position = g_engineProjectionMatrix * gl_Position;
					}

				]
				#frag [
					#version 330 core

					out vec4 color;

					in float depth;

					
					void main(){
						color = vec4(depth,depth,depth,1.0);
					}
				]
			]
		}
	}
}