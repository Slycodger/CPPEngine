#version 450 core

out vec4 Color;

in vec2 inTexCoord;

uniform sampler2D Texture;
uniform vec3 inColor;


void main()
{
    Color = texture(Texture, inTexCoord) * vec4(inColor, 1);
}