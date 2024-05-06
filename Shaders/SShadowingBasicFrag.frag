#version 450 core

out vec4 Color;

in vec4 inColor;
in vec2 inTexCoord;

uniform sampler2D Texture;


void main()
{
	vec4 outColor = inColor;
	vec4 sampled = texture(Texture, inTexCoord);
	outColor = sampled;
    Color = outColor;
}