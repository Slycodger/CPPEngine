#version 450 core

out vec4 Color;
in vec4 inColor;
in vec2 inTexCoord;
uniform sampler2D Texture;

in vec4 Light[2];

uniform vec2 Resolution;
uniform sampler2D DepthTexture[2];

uniform vec3 LightColor[2];

vec4 ShadowValue(int I)
{
	vec3 Val = Light[I].xyz / Light[I].w;
	Val = Val * 0.5 + 0.5;
	float CurrentDepth = Val.z;
	float ClosetDepth = texture(DepthTexture[I], Val.xy).r;
	vec4 shadow = vec4(0);

	float TexelNum = 3;
	float SampleRegion = 0;

	vec2 TexelSize = TexelNum / textureSize(DepthTexture[I], 0);
	int TexelsSampled = 0;
	for(float x = -SampleRegion; x <= SampleRegion; x += TexelNum)
	{
		for(float y = -SampleRegion; y <= SampleRegion; y += TexelNum)
		{
			float TexelDepth = texture(DepthTexture[I], Val.xy + vec2(x,y) * TexelSize).r;
			if(TexelDepth < 1)
			{
				shadow += CurrentDepth - 0.0001f < TexelDepth ? 0.5 * vec4(LightColor[I], 1) : vec4(-0.25, -0.25, -0.25, 1);
				TexelsSampled++;
			}
		}
	}
	shadow /= TexelsSampled;
	shadow.w = 1;


	if(ClosetDepth >= 1.0)
        shadow = vec4(-0.25, -0.25, -0.25, 1);
    
    return shadow;
}

void main()
{
	vec4 outColor = inColor;
	vec4 sampled = texture(Texture, inTexCoord);
	outColor *= sampled;
	for(int i = 0; i < 2; i++)
		outColor += vec4(ShadowValue(i));
    Color = outColor;
}