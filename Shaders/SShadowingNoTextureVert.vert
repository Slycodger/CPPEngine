#version 450 core
layout(location = 0) in vec3 Pos;
layout(location = 2) in vec4 Color;

out vec4 geomColor;

void main()
{
	gl_Position = vec4(Pos, 1);
	geomColor = Color;
}