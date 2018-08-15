ShaderBlock {
	Pass {
		State {
			zwrite off
			ztest always
		}

		SHADER_BEGIN: [ d3d10 d3d11 ]
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

				Texture2D g_engineDepthTexture;
				SamplerState SampleType;
				
				PixelInputType VertMain(VertexInputType input)
				{
				    PixelInputType output;
				    input.position.w = 1.0f;

				    output.position  = mul(g_engineWorldMatrix, input.position);
				    output.position  = mul(g_engineViewMatrix, output.position);
				    output.position = mul(g_engineProjectionMatrix, output.position);
	   
					output.tex = input.tex;
	    
				    return output;
				}

				float4 FragMain(PixelInputType input) : SV_TARGET
				{
				    float4 textureColor = g_engineDepthTexture.Sample(SampleType, input.tex);

	    			return textureColor;
				}
			]
		SHADER_END
		SHADER_BEGIN: [ d3d9 ]
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

				sampler g_engineDepthTexture;

				//float2 offset;

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
				    float4 textureColor = tex2D(g_engineDepthTexture, input.uv);

	    			return textureColor;
				}
			]
		SHADER_END
		SHADER_BEGIN: [ opengl ]
			#code [
				#vert [
					#version 330 core

					out vec4 gl_Position;
					out vec2 uv;

					layout(location = 0) in vec3 input_position;
					layout(location = 1) in vec2 input_texcoord0;

					uniform mat4 g_engineWorldMatrix;
					uniform mat4 g_engineViewMatrix;
					uniform mat4 g_engineProjectionMatrix;

					void main(){
 						gl_Position = g_engineWorldMatrix * vec4(input_position,1);
						gl_Position = g_engineViewMatrix * gl_Position;
						gl_Position = g_engineProjectionMatrix * gl_Position;
						uv = input_texcoord0;
					}

				]
				#frag [
					#version 330 core

					in vec2 uv;

					uniform sampler2D screenTexture;

					uniform vec2 offset;

					out vec4 color;
					void main(){
						color = texture(screenTexture, uv)*0.5;
						color.rgb += texture(screenTexture, uv + offset).rgb*0.15;
						color.rgb += texture(screenTexture, uv - offset).rgb*0.15;
						color.rgb += texture(screenTexture, uv + offset).rgb*0.1;
						color.rgb += texture(screenTexture, uv - offset).rgb*0.1;
						//color = vec4(1.0,0.0,0.0,1.0);

	    				color.a = 1.0;
					}
				]
			]
		SHADER_END
	}
}