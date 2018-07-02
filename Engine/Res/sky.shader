SubShader {
	Desc {
		CompileTarget: { d3d10 d3d11 }
	}
	Pass {
		Tags {
			VertexFunc:   VertMain
			PixelFunc:  FragMain
		}
		State {
			zwrite off
			ztest lequal
		}

		Shader [

			cbuffer MatrixBuffer
			{
				matrix worldMatrix;
				matrix viewMatrix;
				matrix projectionMatrix;
			};

			struct VertexInputType
			{
			    float4 position : POSITION;
			};

			struct PixelInputType
			{
			    float4 position : SV_POSITION;
			    float3 viewDir : TEXCOORD0;
			};

			float3 g_sundir;

			float3 GetSkyColor(float3 dir) {
	float3 col = float3(0,0,0);
	//float3 _SunDir = float3(1.0f, -0.8f, 1.0f);

	float sign = step(0.0, -dir.y);

	float hort = 1.0f - clamp(abs(dir.y), 0.0f, 1.0f);
	//col += 0.5*vec3(0.8,0.9,1)*exp2(hort*8.0-8.0);
	//col += 0.1*vec3(0.8,0.9,1.0)*exp2(hort*3.0-3.0);
	//col += 0.55*vec3(0.7,0.8,1);

	float3 skyC = lerp(float3(0.302, 0.38, 0.537), float3(0.435, 0.545, 0.702), exp2(hort*1.4 - 1.4));
	float3 groundC = lerp(float3(0.412, 0.384, 0.365), float3(0.435, 0.545, 0.702), exp2(hort*4.4*2.7 - 4.4*2.7));
	col += lerp(groundC, skyC, sign);

	col += 0.3*float3(0.8, 0.9, 1.0)*exp2(hort*20.0 - 20.0);
	col += 0.1*float3(0.8, 0.9, 1.0)*exp2(hort*15.0 - 15.0);

	float sun = clamp(dot(normalize(g_sundir.xyz), dir), 0.0, 1.0);
	col += 0.2*float3(1.0, 0.8, 0.2)*pow(sun, 2.0);
	col += 0.5*float3(1.0, 0.8, 0.9)*exp2(sun*650. - 650.0f);
	col += 0.1*float3(1.0, 1.0, 0.8)*exp2(sun*100. - 100.0f);
	col += 0.3*float3(1.0, 0.8, 0.8)*exp2(sun*50. - 50.0f);
	col += 0.5*float3(0.7, 0.3, 0.05)*exp2(sun*10. - 10.0f);

	return col;
}

			PixelInputType VertMain(VertexInputType input)
			{
			    PixelInputType output;
    
    			input.position.w = 1.0f;

    			output.position  = mul(worldMatrix, input.position);
    output.position  = mul(viewMatrix, output.position);
    output.position = mul(projectionMatrix, output.position);

	output.viewDir = -input.position.xyz;
    
    			return output;
			}

			float4 FragMain(PixelInputType input) : SV_TARGET
			{
			    float4 textureColor;
	textureColor.rgb = GetSkyColor(normalize(input.viewDir));
	textureColor.a = 1.0;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    //textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
			}
		]
	}
}
SubShader {
	Desc {
		CompileTarget: { d3d9 }
	}
	Pass {
		Tags {
			VertexFunc:   VertMain
			PixelFunc:  FragMain
		}
		State {
			zwrite off
			ztest lequal
		}

		Shader [

			matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;

struct VS_INPUT
{
    float3 position : POSITION;
};

struct VS_OUTPUT
{
    float4 position : POSITION;
    float3 viewDir  : TEXCOORD0;
};

float3 g_sundir;

float3 GetSkyColor(float3 dir) {
	float3 col = float3(0,0,0);
	//float3 _SunDir = float3(1.0f, -0.8f, 1.0f);

	float sign = step(0.0, -dir.y);

	float hort = 1.0f - clamp(abs(dir.y), 0.0f, 1.0f);
	//col += 0.5*vec3(0.8,0.9,1)*exp2(hort*8.0-8.0);
	//col += 0.1*vec3(0.8,0.9,1.0)*exp2(hort*3.0-3.0);
	//col += 0.55*vec3(0.7,0.8,1);

	float3 skyC = lerp(float3(0.302, 0.38, 0.537), float3(0.435, 0.545, 0.702), exp2(hort*1.4 - 1.4));
	float3 groundC = lerp(float3(0.412, 0.384, 0.365), float3(0.435, 0.545, 0.702), exp2(hort*4.4*2.7 - 4.4*2.7));
	col += lerp(groundC, skyC, sign);

	col += 0.3*float3(0.8, 0.9, 1.0)*exp2(hort*20.0 - 20.0);
	col += 0.1*float3(0.8, 0.9, 1.0)*exp2(hort*15.0 - 15.0);

	float sun = clamp(dot(normalize(g_sundir.xyz), dir), 0.0, 1.0);
	col += 0.2*float3(1.0, 0.8, 0.2)*pow(sun, 2.0);
	col += 0.5*float3(1.0, 0.8, 0.9)*exp2(sun*650. - 650.0f);
	col += 0.1*float3(1.0, 1.0, 0.8)*exp2(sun*100. - 100.0f);
	col += 0.3*float3(1.0, 0.8, 0.8)*exp2(sun*50. - 50.0f);
	col += 0.5*float3(0.7, 0.3, 0.05)*exp2(sun*10. - 10.0f);

	return col;
}

			VS_OUTPUT VertMain(VS_INPUT input)
			{
			    VS_OUTPUT output = (VS_OUTPUT)0;
    
    float4 pos = float4(input.position, 1.0f);

    output.position = mul(pos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.viewDir = -input.position.xyz;
    
    return output;
			}

			float4 FragMain(VS_OUTPUT input) : SV_TARGET
			{
			    float4 textureColor;
	textureColor.rgb = GetSkyColor(normalize(input.viewDir));
	textureColor.a = 1.0;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    //textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
			}
		]
	}
}