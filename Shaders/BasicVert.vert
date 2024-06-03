#version 450 core
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec2 Tex;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 perp;

out vec2 texCoord;


void main()
{
	gl_Position = perp * view * transform * vec4(Pos, 1);
	texCoord = Tex;
}