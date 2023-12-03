#version 330 core
layout (location = 0) in vec3 layoutPosition;
layout (location = 1) in vec3 layoutNormal;
layout (location = 3) in vec2 layoutTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 CurrentPosition;

uniform mat4 model;
uniform mat4 cameraMatrix;

void main()
{
    TexCoords = layoutTexCoords;
    Normal = layoutNormal;
    CurrentPosition = vec3(model * vec4(layoutPosition, 1.0f));

    gl_Position = cameraMatrix * vec4(CurrentPosition, 1.0f);
}