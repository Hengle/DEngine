ShaderBlock {
	Pass {
		State {
			zwrite on
			ztest lequal
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
				    float2 uv : TEXCOORD1; 
				};

				Texture2D g_grabTexture;
				SamplerState SampleType;
				Texture2D noise;
				SamplerState noiseSampleType;

				float speed;
				float g_engineTime;
				float power;
				float2 size;

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
					//output.tex = input.tex;

					output.tex.x = output.position.x / output.position.w * 0.5f + 0.5f;
					output.tex.y = -(output.position.y / output.position.w * 0.5f)+0.5f;

					output.uv = input.tex;
	    
				    return output;
				}

				float4 FragMain(PixelInputType input) : SV_TARGET
				{
					float4 noise1 = noise.Sample(noiseSampleType, input.uv*size+float2(g_engineTime*speed,0.0f));
					float4 noise2 = noise.Sample(noiseSampleType, float2(input.uv.y, 1-input.uv.x)*size+float2(g_engineTime*speed,0.0f));
					float2 n = (noise1.rg+noise2.rg)*0.5f*2-1;
					n*=power;
				    float4 textureColor = g_grabTexture.Sample(SampleType, input.tex+n);

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
				    float2 proj : TEXCOORD1;
				};

				sampler g_grabTexture;
				sampler noise;
				sampler g_engineShadowMap;

				float speed;
				float g_engineTime;
				float power;
				float2 size;

				VS_OUTPUT VertMain(VS_INPUT input)
				{
				    VS_OUTPUT output = (VS_OUTPUT)0;

	    			float4 pos = float4(input.position, 1.0f);

	    			output.position = mul(pos, g_engineWorldMatrix);
	    			output.position = mul(output.position, g_engineViewMatrix);
	    			output.position = mul(output.position, g_engineProjectionMatrix);

	    			output.proj.x = output.position.x / output.position.w * 0.5f + 0.5f;
					output.proj.y = -(output.position.y / output.position.w * 0.5f)+0.5f;

	    			output.uv = input.uv;
	    
	    			return output;
				}

				float4 FragMain(VS_OUTPUT input) : SV_TARGET
				{
					float4 noise1 = tex2D(noise, input.uv*size+float2(g_engineTime*speed,0.0f));
					float4 noise2 = tex2D(noise, float2(input.uv.y, 1-input.uv.x)*size+float2(g_engineTime*speed,0.0f));
					float2 n = (noise1.rg+noise2.rg)*0.5f*2-1;
					n*=power;
				    float4 textureColor = tex2D(g_grabTexture, input.proj+n);

				    //return float4(1,0,0,1);
				    return tex2D(g_grabTexture, input.uv);
				}
			]
		SHADER_END
		SHADER_BEGIN: [ opengl ]
			#code [
				#vert [
					#version 330 core

					out vec4 gl_Position;
					out vec2 uv;
					out vec2 proj;

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

						proj.x = gl_Position.x / gl_Position.w * 0.5 + 0.5;
						proj.y = gl_Position.y / gl_Position.w * 0.5 + 0.5;
					}

				]
				#frag [
					#version 330 core

					in vec2 uv;
					in vec2 proj;

					uniform sampler2D g_grabTexture;
					uniform sampler2D noise;

					uniform float speed;
					uniform float g_engineTime;
					uniform float power;
					uniform vec2 size;

					out vec4 color;
					void main(){
						vec4 noise1 = texture(noise, uv*size+vec2(g_engineTime*speed,0.0));
						vec4 noise2 = texture(noise, vec2(uv.y, 1-uv.x)*size+vec2(g_engineTime*speed,0.0));
						vec2 n = (noise1.xy+noise2.xy)*0.5*2-1;
						n*=power;

						color = texture(g_grabTexture, proj+n);
					}
				]
			]
		SHADER_END
	}
}