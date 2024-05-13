#version 450 core
layout(location = 0) in vec3 Pos;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(Pos, 1);
}