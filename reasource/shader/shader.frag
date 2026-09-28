#version 330 core

out vec4 FragColor;

in vec3 finalColor;
in vec2 TexCoord;

uniform vec4 testColor;
uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, TexCoord);
}