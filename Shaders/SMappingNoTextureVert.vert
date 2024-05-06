#version 450 core

layout (location = 0) in vec3 Pos;
layout (location = 2) in vec4 Color;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 LightView[2];
uniform mat4 perp;
uniform mat4 LightPerp[2];

uniform vec3 MeshColor;

out vec4 inColor;
out vec4 Light[2];

void main()
{
	gl_Position = perp * view * transform * vec4(Pos, 1);
	for(int i = 0; i < 2; i++)
		Light[i] = LightPerp[i] * LightView[i] * transform * vec4(Pos,1);
	inColor = vec4(MeshColor, 1);
}