#version 450 core

out vec4 Color;

in vec2 FragTexCoord;

uniform sampler2D Texture;
uniform vec2 Resolution;

vec4 AveragedColor(vec2 InCoord)
{
    vec4 RetColor = vec4(0);
    int PixelArea = 25;

    for(int x = -PixelArea; x < PixelArea; x++)
    {
        for(int y = -PixelArea; y < PixelArea; y++)
        {
            RetColor += texture(Texture, InCoord + ((x,y) / Resolution));
        }
    }
    RetColor /= PixelArea * PixelArea;

    return RetColor;
}

void main()
{
    Color = AveragedColor(FragTexCoord);
}