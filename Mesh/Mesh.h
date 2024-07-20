#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "../Shaders/Shader.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    glm::vec3    color;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, glm::vec3 color);
    void Draw(Shader& shader, bool updateTextures = true, bool updateColor = true);
    void SetColor(glm::vec3 color) { this->color = color; }
    unsigned int GetVAO() { return VAO; }
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};