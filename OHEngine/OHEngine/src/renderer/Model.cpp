#include "renderer/Model.h"



Model::Model(const char* path)
{
	loadModel(path);
}

void Model::Draw(const Shader & shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader, texturesLoaded);
}

void Model::UpdateTexturesWrap(GLenum dir, GLenum type)
{
	for (int i = 0; i < texturesLoaded.size(); i++)
	{
		texturesLoaded[i].tex.Bind();
		texturesLoaded[i].tex.SetWrap(dir, type);
		texturesLoaded[i].tex.Unbind();
	}

}


void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	// process all the node meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// process all the node children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> textures_indices;

	// loop on all mesh vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		//Position
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		//Normal
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		//texture coordinates
		//if the mesh has tex coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f);
		//Tangent
		if (mesh->mTangents != NULL)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
		}
		//Bitangents
		if (mesh->mBitangents != NULL)
		{
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		vertices.push_back(vertex);
	}
	// loop on faces for indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// get all indices for the face and	add them to the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	// Copied from learnopenGL: we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN
	//1.diffuse maps
	std::vector<unsigned int> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures_indices.insert(textures_indices.end(), diffuseMaps.begin(), diffuseMaps.end());
	//2.specular maps
	std::vector<unsigned int> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures_indices.insert(textures_indices.end(), specularMaps.begin(), specularMaps.end());
	//3.normal maps
	std::vector<unsigned int> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures_indices.insert(textures_indices.end(), normalMaps.begin(), normalMaps.end());
	//4.height maps
	std::vector<unsigned int> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures_indices.insert(textures_indices.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures_indices);
}

std::vector<unsigned int> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType  type, std::string typeName)
{
	std::vector<unsigned int> texture_indices;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		std::string filename = std::string(str.C_Str());
		filename = directory + '/' + filename;

		for (unsigned int j = 0; j < texturesLoaded.size(); j++)
		{
			if (std::strcmp(texturesLoaded[j].tex.GetPath().data(), filename.c_str()) == 0)
			{
				texture_indices.push_back(j);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			//if we didn't load texture yet, load it
			bool isSRGB = typeName == "texture_diffuse";
			Texture tex(filename.c_str(), isSRGB);
			TextureMaterial texture(std::move(tex));
			texture.type = typeName;
			texture_indices.push_back(texturesLoaded.size());
			texturesLoaded.push_back(std::move(texture));
		}
	}
	return texture_indices;
}

Model::~Model()
{
}
