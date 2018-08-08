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
		SHADER_END
		SHADER_BEGIN: [ opengl ]
			#vert VertMain
			#frag FragMain
			#code [
				#vert [

					#version 330 core

					out vec4 gl_Position;
					out vec3 worldPos;
					out vec3 worldNormal;

					layout(location = 0) in vec3 input_position;
					layout(location = 1) in vec3 input_normal;

					uniform mat4 g_engineWorldMatrix;
					uniform mat4 g_engineViewMatrix;
					uniform mat4 g_engineProjectionMatrix;

					void main(){
					    gl_Position = g_engineWorldMatrix * vec4(input_position,1);
					    worldPos = gl_Position.xyz;
					    gl_Position = g_engineViewMatrix * gl_Position;
						gl_Position = g_engineProjectionMatrix * gl_Position;
						worldNormal = mat3(g_engineWorldMatrix) * input_normal;
					}
				]
				#frag [
					#version 330 core

					out vec4 color;

					in vec3 worldPos;
					in vec3 worldNormal;

					uniform vec4 g_engineCameraPos;

					uniform samplerCube shaderTexture;

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
					    vec3 viewDir = GetViewDir(worldPos);
					    vec3 refl = reflect(viewDir, worldNormal);

					    color = vec4(texture(shaderTexture, refl).rgb, 1.0);
					}
				]
				
			]
		SHADER_END
	}
}