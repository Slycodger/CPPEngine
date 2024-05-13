#version 450 core

out vec4 Color;

void main()
{
	if(gl_FrontFacing)
		Color = vec4(0, 1, 0, 1);
	else
		Color = vec4(1, 0, 0, 1);
}