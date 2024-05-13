#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

uniform mat4 view;
uniform mat4 perp;
uniform vec3 LightPos;
uniform float LightExtendDistance;

vec4 GetNormal(vec4 Vert1, vec4 Vert2, vec4 Vert3)
{
	vec4 Ret;

	Ret = vec4(cross((Vert2 - Vert1).xyz, (Vert3 - Vert1).xyz), 0);

	return Ret;
}
vec4 GetOrigin(vec4 Vert1, vec4 Vert2, vec4 Vert3)
{
	vec4 Ret = (Vert1 + Vert2 + Vert3) / 3;
	Ret.w = 0;

	return Ret;
}
vec4 StretchDir(vec4 Vec)
{
	vec4 Ret = normalize(Vec - vec4(LightPos, 0));
	Ret.w = 0;
	return Ret;
}

void PassThrough(vec4 Vert1, vec4 Vert2, vec4 Vert3)
{
    gl_Position = perp * view * Vert1;
    EmitVertex();
    gl_Position = perp * view * Vert2;
    EmitVertex();
    gl_Position = perp * view * Vert3;
    EmitVertex();
    EndPrimitive();
}
void Extend(vec4 Vert1, vec4 Vert2, vec4 Vert3, float StretchAmount)
{
    gl_Position = perp * view * (Vert1 + StretchDir(Vert1) * StretchAmount);
    EmitVertex();
    gl_Position = perp * view * (Vert2 + StretchDir(Vert2) * StretchAmount);
    EmitVertex();
    gl_Position = perp * view * (Vert3 + StretchDir(Vert3) * StretchAmount);
    EmitVertex();
    EndPrimitive();
}

void main()
{
	vec4 Vert1 = gl_in[0].gl_Position;
	vec4 Vert2 = gl_in[1].gl_Position;
	vec4 Vert3 = gl_in[2].gl_Position;

	vec4 CurrentOrigin = GetOrigin(Vert1, Vert2, Vert3);
	vec4 CurrentNormal = GetNormal(Vert1, Vert2, Vert3);

	vec4 CamToOrigin = CurrentOrigin - vec4(LightPos, 0);
	CamToOrigin = normalize(CamToOrigin);
	float Result = dot(CurrentNormal, CamToOrigin);
	if(Result < 0)
	{
		Extend(Vert1, Vert2, Vert3, 100);
	}
	else
	{
		PassThrough(Vert1, Vert2, Vert3);
	}
	
}