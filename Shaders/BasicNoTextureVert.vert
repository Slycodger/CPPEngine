#version 450 core
layout(location = 0) in vec3 Pos;
layout(location = 2) in vec4 Color;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 perp;
uniform vec4 MeshColor;

out vec4 inColor;


void main()
{
	gl_Position = perp * view * transform * vec4(Pos, 1);
	inColor = Color * MeshColor;
}