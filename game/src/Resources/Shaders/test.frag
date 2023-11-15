#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D diffuse0;

void main()
{
    color = vec4(texture(diffuse0, TexCoords));
}