//#version 330 core
//
//in vec2 TexCoords;
//in vec3 Normal;
//in vec3 CurrentPosition;
//
//out vec4 color;
//
//uniform sampler2D diffuse0;
//
//uniform vec4 lightColor0;
//uniform vec3 lightPosition0;
//
//uniform vec4 lightColor1;
//uniform vec3 lightPosition1;
//
//uniform vec4 lightColor2;
//uniform vec3 lightPosition2;
//
//uniform vec4 lightColor3;
//uniform vec3 lightPosition3;
//
//uniform vec4 lightColor4;
//uniform vec3 lightPosition4;
//
//uniform vec3 cameraPosition;
//
//void main()
//{
//    vec4 lightColors[5] = vec4[](lightColor0, lightColor1, lightColor2, lightColor3, lightColor4);
//    vec3 lightPositions[5] = vec3[](lightPosition0, lightPosition0, lightPosition0, lightPosition0, lightPosition0);
//
//    float ambient = 0.20f;
//
//    vec3 normal = normalize(Normal);
//    vec3 lightDirection = normalize(lightPosition0 - CurrentPosition);
//
//    float diffuse = max(dot(normal, lightDirection), 0.0f);
//
//    float specularLight = 0.50f;
//    vec3 viewDirection = normalize(cameraPosition - CurrentPosition);
//    vec3 reflectionDirection = reflect(-lightDirection, normal);
//    float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
//    float specular = specularAmount * specularLight;
//
//    color = texture(diffuse0, TexCoords) * lightColor0 * (diffuse + ambient + specular);
//}

#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 CurrentPosition;

out vec4 color;

uniform sampler2D diffuse0;

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
    vec4 lightColors[5] = vec4[](lightColor0, lightColor1, lightColor2, lightColor3, lightColor4);
    vec3 lightPositions[5] = vec3[](lightPosition0, lightPosition1, lightPosition2, lightPosition3, lightPosition4);

    float ambient = 0.20f;
    float specularLight = 0.50f;

    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(cameraPosition - CurrentPosition);
    

    color = texture(diffuse0, TexCoords);
    // create max light source constant
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