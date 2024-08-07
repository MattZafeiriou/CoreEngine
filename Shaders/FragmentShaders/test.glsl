#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
	vec4 diffuseColor;
	vec3 specularColor;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec4 diffuse;
    vec3 specular;
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec4 diffuse;
	vec3 specular;
};

uniform DirLight dLight;
uniform Light pLights[10]; // needs optimisation
uniform int lightCount;
uniform bool hasTexture;
uniform Material material;
uniform vec3 viewPos;

vec4 calculateDLight(vec3 normal, vec4 diffuseColor, vec3 specularColor, vec3 viewDir) {
    vec3 lightDir = normalize(-dLight.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = dLight.diffuse * diff * diffuseColor;

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dLight.specular * spec * specularColor;

    vec4 result = (diffuse + vec4(specular, diffuse.a));

    return result;
}

vec4 calculatePLights(vec3 normal, vec4 diffuseColor, vec3 specularColor, vec3 viewDir) {
	vec4 result = vec4(0.0);
	for(int i = 0; i < lightCount; i++) {
		vec3 subtract = pLights[i].position - FragPos;
		vec3 lightDir = normalize(subtract);

		float diff = max(dot(normal, lightDir), 0.0);
		vec4 diffuse = pLights[i].diffuse * diff * diffuseColor;

		vec3 reflectDir = reflect(-lightDir, normal);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = pLights[i].specular * spec * specularColor;

		float distance = length(subtract);
		const float c1 = 1.0;
		const float c2 = 0.0;
		const float c3 = 0.01;
		float attenuation = 1.0 / (c1 + c2 * distance + c3 * distance * distance);

		result += (diffuse + vec4(specular, 1.0)) * attenuation;
	}
	return result;
}

void main()
{
	vec3 normal = Normal;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec4 diffuse = material.diffuseColor;
	if (hasTexture) {
	    diffuse = texture(material.diffuse, TexCoords);
	}
	if (diffuse.a < 0.01) {
		discard;
	}
	vec3 specular = material.specularColor;
	if (hasTexture) {
	    specular = vec3(texture(material.specular, TexCoords));
	}
    vec4 result = calculatePLights(normal, diffuse, specular, viewDir);
    result += calculateDLight(normal, diffuse, specular, viewDir);
    // ambient light
    vec4 ambient = vec4(0.05) * diffuse;

    result += ambient;
	result.a = diffuse.a;
    FragColor = result;
}