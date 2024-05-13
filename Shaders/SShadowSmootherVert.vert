#version 450 core
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec4 Color;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 perp;

out vec2 FragTexCoord;


void main()
{
	FragTexCoord = TexCoord;
	gl_Position = perp * view * transform * vec4(Pos, 1);
}