#version 450 core
layout(location = 0) in vec3 Pos;
layout(location = 3) in vec3 Normal;

uniform mat4 PosMat;
uniform mat4 RotMat;
uniform mat4 ScaleMat;
uniform mat4 view;
uniform mat4 perp;
uniform vec3 LightPos;
uniform vec3 CameraPos;
uniform float LED;

out vec4 Color;

void main()
{
	vec4 CurrentPos = PosMat * RotMat * ScaleMat * vec4(Pos, 1);
	vec4 CurrentNormal = RotMat * vec4(Normal, 1);
	vec3 LightToPos = normalize(CurrentPos.xyz - LightPos);
	float RetVal = dot(CurrentNormal.xyz, LightToPos);
	if(RetVal <= -0.05)
		CurrentPos += vec4(LightToPos * LED, 0);
	else
		CurrentPos -= vec4(LightToPos * 0.05f, 0);
	gl_Position = perp * view * CurrentPos;

	Color = vec4(LightToPos, 1);
}