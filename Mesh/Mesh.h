#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "../Shaders/Shader.h"
#include "../Materials/Material.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    // mesh data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material);
    void Draw(Shader& shader, bool updateTextures = true, bool updateColor = true);
    Material* GetMaterial() { return &material; }
    unsigned int GetVAO() { return VAO; }
private:
    //  render data
    unsigned int VAO, VBO, EBO;
    Material material;
    void setupMesh();
};