#version 450 core

out vec4 Color;
in vec4 fragColor;

uniform sampler2D StencilBuffer;
uniform vec2 Resolution;

vec4 ShadowColor()
{
	vec4 Ret = vec4(0);

	if(texture(StencilBuffer, gl_FragCoord.xy / Resolution).r < 0)
		Ret = vec4(-0.25, -0.25, -0.25, 0.4);
	else
		Ret = vec4(0.1);
	return Ret;
}

void main()
{
	Color = fragColor;
}