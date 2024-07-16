#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light dLight;

uniform Material material;
uniform vec3 viewPos;

vec3 calculateDLight() {
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(-dLight.direction);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dLight.specular * spec * vec3(texture(material.specular, TexCoords));

    vec3 result = (diffuse + specular);

    return result;
}

void main()
{
    vec3 result = calculateDLight();
    // ambient light
    vec3 ambient = vec3(0.2) * vec3(texture(material.diffuse, TexCoords));

    result += ambient;
    FragColor = vec4(result, 1.0);
}