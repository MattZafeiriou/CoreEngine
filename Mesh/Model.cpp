#include "Model.h"
#include "../Utils/EnvironmentVariablesUtils.cpp"

vector<Texture> textures_loaded;

Model::Model(const char* path, bool flip)
{
	this->path = path;
	bool debug = std::strcmp(getEnvironmentVariable("CORE_DEBUG"), "1");
	if (debug == 0)
	{
		cout << "Loading model: " << path << endl;
		auto start = std::chrono::high_resolution_clock::now();
		loadModel(path, flip);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		cout << ">> Model loaded in " << elapsed.count() << " seconds" << endl;
	}
	else
	{
		loadModel(path, flip);
	}
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::loadModel(string path, bool flip)
{
	Assimp::Importer importer;
	const aiScene* scene;
	if (flip)
		scene = importer.ReadFile(path, aiProcess_Triangulate
			| aiProcess_JoinIdenticalVertices
			| aiProcess_FlipUVs);
	else
		scene = importer.ReadFile(path, aiProcess_Triangulate
			| aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	glm::vec4 color;
	float minX = 0, minY = 0, minZ = 0, maxX = 0, maxY = 0, maxZ = 0;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if (vector.x < minX)
			minX = vector.x;
		if (vector.y < minY)
			minY = vector.y;
		if (vector.z < minZ)
			minZ = vector.z;
		if (vector.x > maxX)
			maxX = vector.x;
		if (vector.y > maxY)
			maxY = vector.y;
		if (vector.z > maxZ)
			maxZ = vector.z;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}


	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		aiColor4D color_(0.f, 0.f, 0.f, 1.f);
		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color_))
			color = glm::vec4(color_.r, color_.g, color_.b, color_.a);
	}

	Material material(color, glm::vec3(color) * 0.5f, 32.0f, textures);

	return Mesh(vertices, indices, material, glm::vec3(minX, minY, minZ), glm::vec3(maxX, maxY, maxZ));
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}