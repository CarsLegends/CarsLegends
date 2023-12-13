#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 CurrentPosition;
in vec3 Color;

out vec4 color;

uniform sampler2D diffuse0;
uniform sampler2D text;

uniform bool useColor; 
uniform bool useText;

uniform vec4 lightColor0;
uniform vec3 lightPosition0;

uniform vec4 lightColor1;
uniform vec3 lightPosition1;

uniform vec4 lightColor2;
uniform vec3 lightPosition2;

uniform vec4 lightColor3;
uniform vec3 lightPosition3;

uniform vec4 lightColor4;
uniform vec3 lightPosition4;

uniform int amountLights;

uniform vec3 cameraPosition;

void main()
{
    if (useColor) 
    {
        color = vec4(Color, 1.0f);

        if(useText) 
        {
            color *= vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        }

        return;
    }
    else 
    {
        color = texture(diffuse0, TexCoords);
    }

    vec4 lightColors[5] = vec4[](lightColor0, lightColor1, lightColor2, lightColor3, lightColor4);
    vec3 lightPositions[5] = vec3[](lightPosition0, lightPosition1, lightPosition2, lightPosition3, lightPosition4);

    float ambient = 0.40f;
    float specularLight = 0.50f;

    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(cameraPosition - CurrentPosition);

    for (int i = 0; i < amountLights; i++) 
    {
        vec3 lightDirection = normalize(lightPositions[i] - CurrentPosition);
        float diffuse = max(dot(normal, lightDirection), 0.0f);

        vec3 reflectionDirection = reflect(-lightDirection, normal);
        float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
        float specular = specularAmount * specularLight;

        color *= lightColors[i] * (diffuse + specular + ambient);
    }
}