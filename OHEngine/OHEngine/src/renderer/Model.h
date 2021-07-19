#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "renderer/Mesh.h"

class Model
{
public:
	Model(const char* path);
	void Draw(const Shader &shader);
	void UpdateTexturesWrap(GLenum dir, GLenum type);
	~Model();
private:
	//Model data
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<TextureMaterial> texturesLoaded;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<unsigned int> loadMaterialTextures(aiMaterial *mat, aiTextureType  type, std::string typeName);
};

#endif // !MODEL_H
