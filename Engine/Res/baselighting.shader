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
				    float2 tex : TEXCOORD1;
				};

				Texture2D shaderTexture;
				SamplerState SampleType;

				float3 g_engineLightDir;
				float4 g_engineLightColor;

				float4 g_engineCameraPos;

				PixelInputType VertMain(VertexInputType input)
				{
				    PixelInputType output;
	    
				    input.position.w = 1.0f;


				    output.position  = mul(g_engineWorldMatrix, input.position);
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

				    //float3 h = normalize()

				    //float ndh = 


				    textureColor *= ndl;

	    			return textureColor;
				}
				
			]
		}
	}
}