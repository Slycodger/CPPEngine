#version 450 core

out vec4 Color;

uniform sampler2D Texture;

in vec2 texCoord;

void main()
{
    Color = texture(Texture, texCoord);
}