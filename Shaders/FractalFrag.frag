#version 450 core

vec3 palette(float t)
{
    vec3 a = vec3(0.208, 1.708, 0.258);
    vec3 b = vec3(0.826, 1.640, 0.740); 
    vec3 c = vec3(2.000, 4.000, 3.000); 
    vec3 d = vec3(0.000, 0.000, 0.000);

    return a + b * cos(6.28318 * (c * t + d));
}

out vec4 fragColor;

uniform float Time;
uniform vec2 Resolution;

void main()
{
    vec2 uv = (gl_FragCoord.xy * 2 - Resolution.xy) / Resolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0);
    for (int i = 0; i < 3; i++)
    {
        uv = fract(uv * 1.75) - 0.5;
    
        float d = length(uv);
    
        vec3 col = palette(length(uv0) + Time);
    
        d = sin(d * 8 + Time) / 8;
        d = abs(d);
    
        d = 0.02 / d;
    
        finalColor += col * d;
    }
    fragColor = vec4(finalColor, 1.0);
}
