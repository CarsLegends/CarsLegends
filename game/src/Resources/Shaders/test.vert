#version 330 core
layout (location = 0) in vec3 position;
layout (location = 3) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 cameraMatrix;

void main()
{
    TexCoords = texCoords;

    gl_Position = cameraMatrix * model * vec4(position, 1.0f);
}