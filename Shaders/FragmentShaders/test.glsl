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
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dLight;
const int numPLights = 4;
uniform Light pLights[numPLights];

uniform Material material;
uniform vec3 viewPos;

vec3 calculateDLight(vec3 normal) {
    vec3 lightDir = normalize(-dLight.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = dLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dLight.specular * spec * vec3(texture(material.specular, TexCoords));

    vec3 result = (diffuse + specular);

    return result;
}

vec3 calculatePLights(vec3 normal) {
	vec3 result = vec3(0.0);
	for(int i = 0; i < numPLights; i++) {
		vec3 subtract = pLights[i].position - FragPos;
		vec3 lightDir = normalize(subtract);

		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = pLights[i].diffuse * diff * vec3(texture(material.diffuse, TexCoords));

		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, normal);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = pLights[i].specular * spec * vec3(texture(material.specular, TexCoords));

		float distance = length(subtract);
		const float c1 = 1.0;
		const float c2 = 0.0;
		const float c3 = 0.01;
		float attenuation = 1.0 / (c1 + c2 * distance + c3 * distance * distance);

		result += (diffuse + specular) * attenuation;
	}

	return result;
}

void main()
{
	vec3 normal = normalize(Normal);
    vec3 result = calculatePLights(normal);
    result += calculateDLight(normal);
    // ambient light
    vec3 ambient = vec3(0.1) * vec3(texture(material.diffuse, TexCoords));

    result += ambient;
    FragColor = vec4(result, 1.0);
}