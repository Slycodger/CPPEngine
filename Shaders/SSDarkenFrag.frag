#version 450 core

uniform vec4 LightColor;

out vec4 Color;

void main()
{
	Color = LightColor;
}