#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 15) out;

in vec4 geomColor[3];
out vec4 fragColor;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 perp;


void buildHouse(vec4 Pos, vec4 Color)
{
    gl_Position = Pos + vec4(-0.2, -0.2, 0.0, 0.0);
    fragColor = Color;
    EmitVertex();
    gl_Position = Pos + vec4(0.2, -0.2, 0.0, 0.0);
    fragColor = Color;
    EmitVertex();
    gl_Position = Pos + vec4(-0.2,  0.2, 0.0, 0.0);
    fragColor = Color;
    EmitVertex();
    gl_Position = Pos + vec4(0.2,  0.2, 0.0, 0.0);
    fragColor = Color;
    EmitVertex();
    gl_Position = Pos + vec4(0.0,  0.4, 0.0, 0.0);
    fragColor = Color;
    EmitVertex();
    EndPrimitive();
}

float FrontFacing(vec3 x, vec3 y, vec3 z)
{
    vec3 ret = cross(y - x, z - x);
    if(ret.z >= 0)
        return 1;
    return 0;
}

void PassThrough()
{
    vec4 Vert1 = perp * view * transform * (gl_in[0].gl_Position);
    vec4 Vert2 = perp * view * transform * (gl_in[1].gl_Position);
    vec4 Vert3 = perp * view * transform * (gl_in[2].gl_Position);

    gl_Position = Vert1;
    fragColor = geomColor[0];
    EmitVertex();
    gl_Position = Vert2;
    fragColor = geomColor[1];
    EmitVertex();
    gl_Position = Vert3;
    fragColor = geomColor[2];
    EmitVertex();
    EndPrimitive();
}
void main()
{
    PassThrough();
}