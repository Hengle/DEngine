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
				    float2 tex      : TEXCOORD0;
				    float4 tangent  : TANGENT;
				};

				struct PixelInputType
				{
				    float4 position : SV_POSITION;
				    float4 RT0 : TEXCOORD0;
					float4 RT1 : TEXCOORD1;
					float4 RT2 : TEXCOORD2;
				    float2 tex : TEXCOORD3;
				};

				Texture2D shaderTexture;
				Texture2D bumpTexture;
				SamplerState SampleType;
				SamplerState bumpSampler;

				float3 g_engineLightDir;
				float4 g_engineLightColor;

				float4 g_engineCameraPos;

				float gloss;
				float specular;

				PixelInputType VertMain(VertexInputType input)
				{
				    PixelInputType output;
	    
				    input.position.w = 1.0f;


				    output.position  = mul(g_engineWorldMatrix, input.position);
				    float3 worldPos = output.position.xyz;
				    output.position  = mul(g_engineViewMatrix, output.position);
				    output.position = mul(g_engineProjectionMatrix, output.position);

				   	float3 worldNormal = mul(g_engineWorldMatrix, float4(input.normal, 0.0f)).xyz;
				   	float3 worldTan = mul(g_engineWorldMatrix, input.tangent).xyz;

				   	float3 worldBinormal = cross(worldNormal, worldTan).xyz;
	    
					output.tex = input.tex;

					output.RT0 = float4(worldTan.x, worldBinormal.x, worldNormal.x, worldPos.x);
					output.RT1 = float4(worldTan.y, worldBinormal.y, worldNormal.y, worldPos.y);
					output.RT2 = float4(worldTan.z, worldBinormal.z, worldNormal.z, worldPos.z);
	    
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
				    float3 rnormal = bumpTexture.Sample(bumpSampler, input.tex).rgb;

				    rnormal = rnormal * 2.0f - 1.0f;

				    float3 worldPos = float3(input.RT0.w,input.RT1.w,input.RT2.w);
				    float3 worldNormal = float3(dot(input.RT0.xyz, rnormal), dot(input.RT1.xyz, rnormal), dot(input.RT2.xyz, rnormal));

				    float ndl = max(0.0f, dot(worldNormal, -g_engineLightDir));

				    float3 h = normalize(GetViewDir(worldPos) - g_engineLightDir);

				    float ndh = max(0.0f, dot(worldNormal, h));

				    //textureColor.rgb =  textureColor.rgb * ndl;
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
					out vec4 RT0;
					out vec4 RT1;
					out vec4 RT2;
				    out vec2 tex;

					layout(location = 0) in vec3 input_position;
					layout(location = 1) in vec3 input_normal;
					layout(location = 2) in vec2 input_texcoord0;
					layout(location = 3) in vec4 input_tangent;

					uniform mat4 g_engineWorldMatrix;
					uniform mat4 g_engineViewMatrix;
					uniform mat4 g_engineProjectionMatrix;

					void main(){
					    gl_Position = g_engineWorldMatrix * vec4(input_position,1);
					    vec3 worldPos = gl_Position.xyz;
					    gl_Position = g_engineViewMatrix * gl_Position;
						gl_Position = g_engineProjectionMatrix * gl_Position;
						vec3 worldNormal = mat3(g_engineWorldMatrix) * input_normal;
						tex = input_texcoord0;

						vec3 worldTan = mat3(g_engineWorldMatrix) * input_tangent.xyz;
						vec3 worldBinormal = cross(worldNormal, worldTan).xyz;

						RT0 = vec4(worldTan.x, worldBinormal.x, worldNormal.x, worldPos.x);
						RT1 = vec4(worldTan.y, worldBinormal.y, worldNormal.y, worldPos.y);
						RT2 = vec4(worldTan.z, worldBinormal.z, worldNormal.z, worldPos.z);

					}
				]
				#frag [
					#version 330 core

					out vec4 color;

					in vec4 RT0;
					in vec4 RT1;
					in vec4 RT2;
				    in vec2 tex;

					uniform vec3 g_engineLightDir;
					uniform vec4 g_engineLightColor;

					uniform vec4 g_engineCameraPos;

					uniform float gloss;
					uniform float specular;

					uniform sampler2D shaderTexture;
					uniform sampler2D bumpTexture;

					vec3 GetViewDir(vec3 worldPos) {
						vec3 viewdir;
						if(g_engineCameraPos.w == 0.0) {
							viewdir = -g_engineCameraPos.xyz;
						}else{
							viewdir = normalize(g_engineCameraPos.xyz - worldPos);
						}
						return -viewdir;
					}

					void main(){
					    color = texture(shaderTexture, tex);
					    vec3 rnormal = texture(bumpTexture, tex).rgb;

					    rnormal = rnormal * 2.0 - 1.0;

					    vec3 worldPos = vec3(RT0.w,RT1.w,RT2.w);
				    	vec3 worldNormal = vec3(dot(RT0.xyz, rnormal), dot(RT1.xyz, rnormal), dot(RT2.xyz, rnormal));
					    // //color = vec4(uv,0.0,1.0);

					    float ndl = max(0.0, dot(worldNormal, -g_engineLightDir));
					    // //float ndl = dot(worldNormal, g_engineLightDir)*0.5+0.5;

					    vec3 h = normalize(GetViewDir(worldPos) - g_engineLightDir);

					    float ndh = max(0.0, dot(worldNormal, h));

					    //color.rgb = color.rgb * ndl;
					    color.rgb =  color.rgb * ndl + vec3(1.0,1.0,1.0)*pow(ndh,specular*128.0)*gloss;
					    color.a = 1.0;
					    // color.rgb = vec3(RT0.x, RT1.x, RT2.x);
					    //color.rgb = vec3(uv,worldNormal.x);

					}
				]
				
			]
		}
	}
}