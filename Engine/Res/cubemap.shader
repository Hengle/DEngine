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
				};

				struct PixelInputType
				{
				    float4 position : SV_POSITION;
				    float3 worldNormal:TEXCOORD0;
				    float3 worldPosition:TEXCOORD1;
				};

				TextureCube shaderTexture;
				SamplerState SampleType;

				float4 g_engineCameraPos;

				PixelInputType VertMain(VertexInputType input)
				{
				    PixelInputType output;
	    
				    input.position.w = 1.0f;


				    output.position  = mul(g_engineWorldMatrix, input.position);
				    output.worldPosition = output.position.xyz;
				    output.position  = mul(g_engineViewMatrix, output.position);
				    output.position = mul(g_engineProjectionMatrix, output.position);
	    			
	    			output.worldNormal = mul(g_engineWorldMatrix, float4(input.normal, 0.0f)).xyz;
	    
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
				    float3 viewdir = GetViewDir(input.worldPosition);
				    float3 refl = reflect(viewdir, input.worldNormal);

				    float4 textureColor = shaderTexture.Sample(SampleType, refl);

	    			return textureColor;
				}
				
			]
		}
	}
}