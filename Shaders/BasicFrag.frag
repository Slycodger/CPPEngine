#version 450 core

out vec4 Color;

in vec2 inTexCoord;

uniform sampler2D Texture;


void main()
{
    Color = texture(Texture, inTexCoord);
}