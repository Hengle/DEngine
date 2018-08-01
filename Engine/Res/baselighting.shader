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
				    float3 normal   : NORMAL;
				    float2 tex : TEXCOORD0;
				};

				struct PixelInputType
				{
				    float4 position : SV_POSITION;
				    float3 worldNormal:TEXCOORD0;
				    float3 worldPosition:TEXCOORD1;
				    float2 tex : TEXCOORD2;
				};

				Texture2D shaderTexture;
				SamplerState SampleType;

				float3 g_engineLightDir;
				float4 g_engineLightColor;

				float4 g_engineCameraPos;

				float g_engineTime;

				float gloss;
				float specular;

				PixelInputType VertMain(VertexInputType input)
				{
				    PixelInputType output;
	    
				    input.position.w = 1.0f;


				    output.position  = mul(g_engineWorldMatrix, input.position);
				    output.worldPosition = output.position.xyz;
				    output.position  = mul(g_engineViewMatrix, output.position);
				    output.position = mul(g_engineProjectionMatrix, output.position);

				    output.worldNormal = mul(g_engineWorldMatrix, float4(input.normal, 0.0f)).xyz;
	    
					output.tex = input.tex;
	    
				    return output;
				}

				float3 GetViewDir(float3 worldPos) {
					float3 viewdir;
					if(g_engineCameraPos.w == 0.0f) {
						viewdir = -g_engineCameraPos.xyz;
					}else{
						viewdir = normalize(g_engineCameraPos.xyz - worldPos);
					}
					return viewdir;
				}

				float4 FragMain(PixelInputType input) : SV_TARGET
				{
				    float4 textureColor = shaderTexture.Sample(SampleType, input.tex);

				    float ndl = max(0.0f, dot(input.worldNormal, -g_engineLightDir));

				    float3 h = normalize(GetViewDir(input.worldPosition) - g_engineLightDir);

				    float ndh = max(0.0f, dot(input.worldNormal, h));


				    textureColor.rgb =  textureColor.rgb * ndl + float3(1.0f,1.0f,1.0f)*pow(ndh,specular*128.0f)*gloss;
				    textureColor.a = 1.0f;

	    			return textureColor;
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
			#vert VertMain
			#frag FragMain
			#code [
				#vert [

					#version 330 core

					out vec4 gl_Position;
					out vec3 worldNormal;
					out vec3 worldPosition;
					out vec2 uv;

					layout(location = 0) in vec3 input_position;
					layout(location = 1) in vec3 input_normal;
					layout(location = 2) in vec2 input_texcoord0;

					uniform mat4 g_engineWorldMatrix;
					uniform mat4 g_engineViewMatrix;
					uniform mat4 g_engineProjectionMatrix;

					void main(){
					    gl_Position = g_engineWorldMatrix * vec4(input_position,1);
					    worldPosition = gl_Position.xyz;
					    gl_Position = g_engineViewMatrix * gl_Position;
						gl_Position = g_engineProjectionMatrix * gl_Position;
						worldNormal = mat3(g_engineWorldMatrix) * input_normal;
						uv = input_texcoord0;
					}
				]
				#frag [
					#version 330 core

					out vec4 color;

					in vec3 worldNormal;
					in vec3 worldPosition;
					in vec2 uv;

					uniform vec3 g_engineLightDir;
					uniform vec4 g_engineLightColor;

					uniform vec4 g_engineCameraPos;

					uniform float g_engineTime;

					uniform float gloss;
					uniform float specular;

					uniform sampler2D shaderTexture;

					vec3 GetViewDir(vec3 worldPos) {
						vec3 viewdir;
						if(g_engineCameraPos.w == 0.0) {
							viewdir = -g_engineCameraPos.xyz;
						}else{
							viewdir = normalize(g_engineCameraPos.xyz - worldPos);
						}
						return viewdir;
					}

					void main(){
					    color = texture(shaderTexture, uv+vec2(g_engineTime*0.1,0.0));
					    //color = vec4(uv,0.0,1.0);

					    float ndl = max(0.0, dot(worldNormal, -g_engineLightDir));
					    //float ndl = dot(worldNormal, g_engineLightDir)*0.5+0.5;

					    vec3 h = normalize(GetViewDir(worldPosition) - g_engineLightDir);

					    float ndh = max(0.0, dot(worldNormal, h));

					    //color.rgb = color.rgb * ndl;
					    color.rgb =  color.rgb * ndl + vec3(1.0,1.0,1.0)*pow(ndh,specular*128.0)*gloss;
					    color.a = 1.0;

					    //color.rgb = vec3(uv,worldNormal.x);
					}
				]
				
			]
		}
	}
}