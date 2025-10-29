//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screen_texture;

void main() {
    vec3 color = texture(screen_texture, TexCoords).rgb;
    float contrast = 1.2;
    color = (color - 0.5) * contrast + 0.5;

    vec3 tint = vec3(1.0, 0.75, 0.7);
    color *= tint;

    /*float grayFactor = 0.7;
    float luminance = dot(color, vec3(0.2, 0.7, 0.07));
    color = mix(color, vec3(luminance), grayFactor);

    vec2 pos = TexCoords - 0.5;
    float len = length(pos);
    float vignette = smoothstep(0.8, 0.5, len);
    color *= vignette;*/

    FragColor = vec4(color, 1.0f);
}