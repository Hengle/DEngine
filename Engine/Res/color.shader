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

			    	output.position  = mul(g_worldMatrix, vpos);
			    	output.position  = mul(g_viewMatrix, output.position);
			    	output.position = mul(g_projectionMatrix, output.position);

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

    				output.position = mul(pos, g_worldMatrix);
    				output.position = mul(output.position, g_viewMatrix);
    				output.position = mul(output.position, g_projectionMatrix);

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
					out vec4 fragmentColor;

					layout(location = 0) in vec3 input_position;
					layout(location = 1) in vec4 input_color;

					uniform mat4 g_worldMatrix;
					uniform mat4 g_viewMatrix;
					uniform mat4 g_projectionMatrix;

					void main(){
 						gl_Position = g_worldMatrix * vec4(input_position,1);
						gl_Position = g_viewMatrix * gl_Position;
						gl_Position = g_projectionMatrix * gl_Position;
						fragmentColor = input_color;
					}

				]
				#frag [
					#version 330 core

					in vec4 fragmentColor;

					out vec4 color;
					void main(){
						color = fragmentColor;
					}
				]
			]
		}
	}
}