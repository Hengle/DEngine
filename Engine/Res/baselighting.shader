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

				    float3 h = normalize(GetViewDir(input.worldPosition - g_engineLightDir));

				    float ndh = max(0.0f, dot(input.worldNormal, h));


				    textureColor.rgb =  textureColor.rgb * ndl + float3(1.0f,1.0f,1.0f)*pow(ndh,specular*128.0f)*gloss;
				    textureColor.a = 1.0f;

	    			return textureColor;
				}
				
			]
		}
	}
}