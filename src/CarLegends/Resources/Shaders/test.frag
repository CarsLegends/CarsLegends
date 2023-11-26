#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D diffuse0;
uniform vec4 Color;

void main()
{
    color = vec4(texture(diffuse0, TexCoords));
    //color = vec4(1.0f,1.0f,1.0f,.7f) + Color;
}