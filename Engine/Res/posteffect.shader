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
			ztest always
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
			    float2 tex : TEXCOORD0;
			};

			struct PixelInputType
			{
			    float4 position : SV_POSITION;
			    float2 tex : TEXCOORD0;
			};

			Texture2D screenTexture;
			SamplerState SampleType;
			
			PixelInputType VertMain(VertexInputType input)
			{
			    PixelInputType output;
    

				// Change the position vector to be 4 units for proper matrix calculations.
			    input.position.w = 1.0f;

				// Calculate the position of the vertex against the world, view, and projection matrices.

			    output.position  = mul(worldMatrix, input.position);
			    output.position  = mul(viewMatrix, output.position);
			    output.position = mul(projectionMatrix, output.position);
    
				// Store the texture coordinates for the pixel shader.
				output.tex = input.tex;
    
			    return output;
			}

			float4 FragMain(PixelInputType input) : SV_TARGET
			{
			    float4 textureColor;


    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = screenTexture.Sample(SampleType, input.tex);
    textureColor.r = 1.0f - textureColor.r;

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
			ztest always
		}

		Shader [

			matrix worldMatrix;
			matrix viewMatrix;
			matrix projectionMatrix;

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

			sampler screenTexture;

			VS_OUTPUT VertMain(VS_INPUT input)
			{
			    VS_OUTPUT output = (VS_OUTPUT)0;

    			float4 pos = float4(input.position, 1.0f);

    			output.position = mul(pos, worldMatrix);
    			output.position = mul(output.position, viewMatrix);
    			output.position = mul(output.position, projectionMatrix);

    			output.uv = input.uv;
    
    			return output;
			}

			float4 FragMain(VS_OUTPUT input) : SV_TARGET
			{
			    float4 c = tex2D(screenTexture,      input.uv);
    c.r = 1.0 - c.r;
    return c;
			}
		]
	}
}